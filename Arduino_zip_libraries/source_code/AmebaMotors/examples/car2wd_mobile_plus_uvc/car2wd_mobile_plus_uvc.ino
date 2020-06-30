/*
  This sketch use mobile phone to control car with streaming video.

  At first Ameba connect to AP.
  Then Ameba open UVC service.
  After UVC service is enabled, user can use rtsp player and open streaming with address:
    rtsp://192.168.1.123/test.sdp
  (NOTE: the IP address depends on Ameba's IP")

  For car related control method, please refer example car2wd_mobile_control
*/

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Car2wd.h>
#include <UVC.h>

char ssid[] = "mycar";        // Set the AP's SSID
char pass[] = "12345678";     // Set the AP's password
char channel[] = "6";         // Set the AP's channel
int status = WL_IDLE_STATUS;  // the Wifi radio's status

Car2wd car(8,9,10,11,12,13);

int port_ctrl = 5001;
#define CTRL_USE_TCP
#ifdef CTRL_USE_TCP
WiFiServer server(port_ctrl);
WiFiClient client;
#else
WiFiUDP udp;
#endif

uint8_t buffer[256];

void setup() {
  car.begin();

  status = WiFi.status();

  // attempt to start AP:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to start AP with SSID: ");
    Serial.println(ssid);
    status = WiFi.apbegin(ssid, pass, channel);
    if (status == WL_CONNECTED) {
      break;
    }
    delay(10000);
  }

  //AP MODE already started:
  Serial.println("AP mode already started");

  // Default setting is motion jpeg with 320x240 resolution and frame rate is 30fps
  UVC.begin();
  // Try below setting if you want better resolution
  //UVC.begin(UVC_MJPEG, 640, 480, 30, 0);

  // wait until UVC is ready for streaming
  while (!UVC.available()) {
    delay(100);
  }
  Serial.println("UVC is ready");
  Serial.println("Open your rtsp player with this address:");
  Serial.print("\trtsp://");
  Serial.print(WiFi.localIP());
  Serial.println("/test.sdp");

#ifdef CTRL_USE_TCP
  server.begin();
#else
  udp.begin(port_ctrl);
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
      printf("[TCP]:%s\r\n", buffer);
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
      printf("[UDP]:%s\r\n", buffer);
      buffer[len] = '\0';
    }
    handleData((const char *)buffer);
    delay(100);
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

int lastXspeed = 0;
int lastYspeed = 0;
void handleData(const char *buf) {
  int len;
  bool xchange = false, ychange = false;;
  int xspeed = 0, yspeed = 0;

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
    if (xchange == false) {
      xspeed = lastXspeed;
    }
    if (ychange == false) {
      yspeed = lastYspeed;
    }
  } else {
    xspeed = lastXspeed = 0;
    yspeed = lastYspeed = 0;
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