#include <WiFi.h>
#include <WiFiUdp.h>
#include <Car2wd.h>
#include <UVC.h>

#define NO_CONTROLLER  0
#define CONTROLLER_1   1
#define CONTROLLER_2   2

char ssid[] = "mycar";        // Set the AP's SSID
char pass[] = "12345678";     // Set the AP's password
char channel[] = "11";        // Set the AP's channel
int status = WL_IDLE_STATUS;  // the Wifi radio's status

/* control 1 */
int port_ctrl1 = 5001;
//#define CTRL1_USE_TCP
#ifdef CTRL1_USE_TCP
WiFiServer server(port_ctrl1);
#else
// Use UDP in control 1
WiFiUDP udp1;
#endif

/* control 2 */
WiFiUDP udp2;
int port_ctrl2 = 5002;

Car2wd car(8,9,10,11,12,13);

#define DBG (1)

void setup() {

  car.begin();

  status = WiFi.status();
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to start AP with SSID: ");
    Serial.println(ssid);
    status = WiFi.apbegin(ssid, pass, channel);
    if (status == WL_CONNECTED) break;
    delay(1000);
  }
  Serial.println("AP mode already started");

  UVC.begin();
  while (!UVC.available()) delay(1000);
  Serial.println("UVC is ready");

#ifdef CTRL1_USE_TCP
  server.begin();
  os_thread_create(tcp_ctrl_task, NULL, OS_PRIORITY_REALTIME, 2048);
#else
  udp1.begin(port_ctrl1);
  os_thread_create(udp1_ctrl_task, NULL, OS_PRIORITY_REALTIME, 2048);
#endif

  udp2.begin(port_ctrl2);
  os_thread_create(udp2_ctrl_task, NULL, OS_PRIORITY_REALTIME, 2048);

}

#ifdef CTRL1_USE_TCP
uint8_t tcpbuf[256];
void tcp_ctrl_task(const void *argument) {
  int len;
  WiFiClient client;
  while (1) {
    if (!client.connected()) {
      client = server.available();
    }
    while (client.connected()) {
      client.setRecvTimeout(1800);
      memset(tcpbuf, 0, sizeof(tcpbuf));
      len = client.read(tcpbuf, sizeof(tcpbuf)-1);
      if (len > 0) {
        if (DBG) printf("[TCP]:%s\r\n", tcpbuf);
        tcpbuf[len] = '\0';
      }
      handleData((const char *)tcpbuf, CONTROLLER_1);
    }
    delay(100);
  }
}
#else
uint8_t udpbuf1[256];
void udp1_ctrl_task(const void *argument) {
  int len;
  while(1) {
    memset(udpbuf1, 0, sizeof(udpbuf1));
    len = udp1.read(udpbuf1, sizeof(udpbuf1)-1);
    if (len > 0) {
      if (DBG) printf("[UDP1]:%s\r\n", udpbuf1);
      udpbuf1[len] = '\0';
    }
    handleData((const char *)udpbuf1, CONTROLLER_1);
    delay(100);
  }
}
#endif

uint8_t udpbuf2[256];
void udp2_ctrl_task(const void *argument) {
  int len;
  while(1) {
    memset(udpbuf2, 0, sizeof(udpbuf2));
    len = udp2.read(udpbuf2, sizeof(udpbuf2)-1);
    if (len > 0) {
      if (DBG) printf("[UDP2]:%s\r\n", udpbuf2);
      udpbuf2[len] = '\0';
    }
    handleData((const char *)udpbuf2, CONTROLLER_2);
    delay(100);
  }
}

void loop() {
  delay(1000);
}

int speedMapping(int rawspeed) {
  /* Original data range: -12 ~ + 12
   * Convert it to PWM range: 150~255
   **/
  if (rawspeed == 0) {
    return 0;
  } else {
    return (rawspeed * 105) / 12 + ((rawspeed > 0) ? 150 : -150);
  }
}

#define NO_CONTROL_TIMEOUT 2000
int currentController = NO_CONTROLLER;
uint32_t lastActionTimestamp = 0;
int lastXspeed = 0;
int lastYspeed = 0;
void handleData(const char *buf, int controller) {

  int len;
  bool xchange = false, ychange = false;;
  int xspeed = 0, yspeed = 0;
  uint32_t currentTimestamp;

  currentTimestamp = millis();

  if ((currentController != NO_CONTROLLER) && (currentController != controller)) {
    if (currentTimestamp - lastActionTimestamp > NO_CONTROL_TIMEOUT) {
      // There is other controller, but there is no action for a while. Switch control to current controller.
      currentController = controller;
    } else {
      return;
    }
  }

  if (buf[0] != '\0') {
    len = strlen(buf);
    for (int i=0; i<len; i++) {
      if (buf[i] == 'X') {
        xspeed = lastXspeed = speedMapping(parseSpeed(&buf[i+2]));
        xchange = true;
      }
      if (buf[i] == 'Y') {
        yspeed = lastYspeed = speedMapping(parseSpeed(&buf[i+2]));
        ychange = true;
      }
    }
    if (xchange == false) xspeed = lastXspeed;
    if (ychange == false) yspeed = lastYspeed;
  } else {
    xspeed = lastXspeed = 0;
    yspeed = lastYspeed = 0;
  }

  if (xspeed != 0 || yspeed != 0) {
    lastActionTimestamp = currentTimestamp;
    if (currentController == NO_CONTROLLER) {
      currentController = controller;
    }
  } else {
    if (currentTimestamp - lastActionTimestamp > NO_CONTROL_TIMEOUT) {
      currentController = NO_CONTROLLER;
    }
  }

  if (xspeed == 0 && yspeed == 0) {
    car.stop();
  } else if (xspeed == 0 && yspeed > 0) {
    car.setAction(CAR_FORWARD, abs(yspeed));
  } else if (xspeed == 0 && yspeed < 0) {
    car.setAction(CAR_BACKWARD, abs(yspeed));
  } else if (xspeed > 0 && yspeed < 0 ) {
    car.setAction(CAR_BACKWARD_RIGHT, abs(yspeed));
  } else if (xspeed > 0 && yspeed == 0) {
    car.setAction(CAR_ROTATE_RIGHT, abs(xspeed));
  } else if (xspeed > 0 && yspeed > 0 ) {
    car.setAction(CAR_FORWARD_RIGHT, abs(yspeed));
  } else if (xspeed < 0 && yspeed < 0 ) {
    car.setAction(CAR_BACKWARD_LEFT, abs(yspeed));
  } else if (xspeed < 0 && yspeed == 0) {
    car.setAction(CAR_ROTATE_LEFT, abs(xspeed));
  } else if (xspeed < 0 && yspeed > 0 ) {
    car.setAction(CAR_FORWARD_LEFT, abs(yspeed));
  }
}

int parseSpeed(const char *buf) {
  int s = 0;
  if (buf[0] == '-') {
    s = buf[1] - '0';
    if (buf[2] >= '0' && buf[2] <= '9') {
      s = s * 10 + (buf[2] - '0');
    }
    s = -s;
  } else {
    s = buf[0] - '0';
    if (buf[1] >= '0' && buf[1] <= '9') {
      s = s * 10 + (buf[1] - '0');
    }
  }
  return s;
}