/*

 This sketch require these items:
   - Ameba x 1
   - PCA9685 x 1
   - Thunder Tiger Jackal car x 1

 There are 2 components need to be control in Thunder Tiger.
 One is direction servo. This servo require a 6V power source. And the servo working level is 3.3V.
 The servo works at 54Hz, and the range of pulse width is 1ms~2ms.
 The other is motor control. The power line provide 6V power source.
 The signal line use pwm with same property as direction servo.

 The PCA9685 is for smooth the action.

 The direction servo may drain power from low to high rapidly. So we need a 100uF capacity.
 So the whole connection is as below:

    Motor power line --- Ameba 5V pin --- 100uF capacity positive side --- direction servo power line
    Motor GND --- Ameba GND --- 100uF capacity negative side --- direction servo GND

    Ameba 3.3V pin --- PCA9685 VCC
    Ameba GND --- PCA9685 GND
    Ameba SDA --- PCA9685 SDA
    Ameba SCL --- PCA9685 SCL

    PCA9685 PWM 0 signal --- Motor signal line
    PCA9685 PWM 1 signal --- direction servo signal line

 */

#include <WiFi.h>
#include <WiFiUdp.h>
#include <UVC.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define DBG (1)
//#define CTRL_USE_TCP
#define ENABLE_UVC

char ssid[] = "myJackal";     // Set the AP's SSID
char pass[] = "12345678";     // Set the AP's password
char channel[] = "6";         // Set the AP's channel
int status = WL_IDLE_STATUS;  // the Wifi radio's status

int port_ctrl = 5001;
#ifdef CTRL_USE_TCP
WiFiServer server(port_ctrl);
WiFiClient client;
#else
WiFiUDP udp;
#endif

#define PWM_THR 0
#define PWM_STR 1
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int ctrl_range_min = -127;
const int ctrl_range_max = +127;

const int xspeed_min  = 221; // 4096 *  5.383 / 100
const int xspeed_stop = 307; // 4096 *  8.013 / 100
const int xspeed_max  = 439; // 4096 * 10.724 / 100

const int yspeed_break  = 213; // 4096 *  5.190 / 100 (Full Speed)
const int yspeed_min  = 300; // (Limited Speed)
//const int yspeed_min  = 213; // 4096 *  5.190 / 100 (Full Speed)
const int yspeed_stop = 328; // 4096 *  8.018 / 100
const int yspeed_max  = 354; // (Limited Speed)
//const int yspeed_max  = 434; // 4096 * 10.600 / 100 (Full Speed)

uint8_t buffer[256];

void setup() {
  pwm.begin();
  pwm.setPWMFreq(54);
  pwm.setPWM(PWM_THR, 0, yspeed_stop);
  pwm.setPWM(PWM_STR, 1, xspeed_stop);

  status = WiFi.status();
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to start AP with SSID: ");
    Serial.println(ssid);
    status = WiFi.apbegin(ssid, pass, channel);
    if (status == WL_CONNECTED) break;
    delay(1000);
  }
  Serial.println("AP mode already started");

#ifdef CTRL_USE_TCP
  server.begin();
#else
  udp.begin(port_ctrl);
#endif

#ifdef ENABLE_UVC
  UVC.begin();
  while (!UVC.available()) delay(100);
  Serial.println("UVC is ready");
#endif
}

#ifdef CTRL_USE_TCP
void tcpHandler() {
  int len;
  if (!client.connected()) {
    client = server.available();
  }
  while (client.connected()) {
    memset(buffer, 0, 256);
    len = client.read(buffer, sizeof(buffer)-1);
    if (len > 0) {
      if (DBG) printf("%s\r\n", buffer);
      buffer[len] = '\0';
    }
    handleData((const char *)buffer);
  }
  Serial.println("control socket broken");
  memset(buffer, 0, 256);
  handleData((const char *)buffer);
  delay(1000);  
}
#else
void udpHandler() {
  int len;
  while(1) {
    memset(buffer, 0, sizeof(buffer));
    len = udp.read(buffer, sizeof(buffer)-1);
    if (len > 0) {
      if (DBG) printf("%s\r\n", buffer);
      buffer[len] = '\0';
    }
    handleData((const char *)buffer);
  }
}
#endif

void loop() {
#ifdef CTRL_USE_TCP
  tcpHandler();
#else
  udpHandler();
#endif
}

int thrMapping(int rawspeed) {
  if (rawspeed == 0) {
    return yspeed_stop;
  } else {
    return yspeed_min + (rawspeed - ctrl_range_min) * (yspeed_max - yspeed_min) / (ctrl_range_max - ctrl_range_min);
  }
}

int strMapping(int rawspeed) {
  return xspeed_min + (rawspeed - ctrl_range_min) * (xspeed_max - xspeed_min) / (ctrl_range_max - ctrl_range_min);
}

int lastXspeed = xspeed_stop;
int lastYspeed = yspeed_stop;
int breakCounter = 0;
void handleData(const char *buf) {
  int len;
  bool xchange = false, ychange = false;;
  int xspeed = lastXspeed;
  int yspeed = yspeed_stop;

  if (buf[0] != '\0') {
    len = strlen(buf);

    if (strcmp(buf, "R?") == 0) {
      char rangebuf[64];
      sprintf(rangebuf, "R=X:%d,%d;Y%d,%d", ctrl_range_min, ctrl_range_max, ctrl_range_min, ctrl_range_max);
#ifdef CTRL_USE_TCP
      client.write(rangebuf, strlen(rangebuf));
#else
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(rangebuf, strlen(rangebuf));
      udp.endPacket();
#endif
      return;
    }

    for (int i=0; i<len; i++) {
      if (buf[i] == 'X') {
        xspeed = lastXspeed = strMapping(atoi(&buf[i+2]));
        xchange = true;
      }
      if (buf[i] == 'Y') {
        yspeed = lastYspeed = thrMapping(atoi(&buf[i+2]));
        ychange = true;
      }
    }
    if (xchange == false) {
      xspeed = lastXspeed;
    }
    if (ychange == false) {
      yspeed = lastYspeed;
    }
  } else {
    yspeed = lastYspeed = yspeed_stop;
  }

  if (yspeed == yspeed_min && lastYspeed > yspeed_stop + 10) {
    breakCounter = 3;
  }
  if (breakCounter > 0) {
    if (yspeed == yspeed_min) {
      yspeed = yspeed_break;
      breakCounter--;
    } else {
      breakCounter = 0;
    }
  }

  if (DBG) printf("(%d,%d)\r\n", xspeed, yspeed);
  pwm.setPWM(PWM_THR, 0, yspeed);
  pwm.setPWM(PWM_STR, 0, xspeed);
}