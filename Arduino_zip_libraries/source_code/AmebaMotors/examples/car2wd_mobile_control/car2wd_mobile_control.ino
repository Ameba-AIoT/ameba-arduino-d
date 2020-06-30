/*
 * This sketch use mobile phone to control car
 *
 * This sketch bring up wifi in AP mode.
 * User can change mobile phone's wifi AP with correspond ssid.
 * Device open TCP socket on port 5001, and mobile app connect to
 * it on default AP IP 192.168.1.1
 * Mobile app send raw data when control data is changed.
 * The Raw data has these format:
 *     <direction>:<speed>
 *
 *     Y:12
 *     It's forward with max speed. (Speed has range -12~12)
 *
 *     Y:-6
 *     It's backward with half speed.
 *
 *     X:12
 *     It's right rotate with max speed.
 *
 *     Y:12
 *     X:-6
 *     It's forward left with max speed. (When X and Y has speed value, use Y speed only)
 *     
 **/

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Car2wd.h>

char ssid[] = "mycar";        // Set the AP's SSID
char pass[] = "12345678";     // Set the AP's password
char channel[] = "6";         // Set the AP's channel
int status = WL_IDLE_STATUS;  // the Wifi radio's status

Car2wd car(8, 9, 10, 11, 12, 13);

int port_ctrl = 5001;
//#define CTRL_USE_TCP
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
        len = client.read(buffer, (sizeof(buffer) - 1));
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
        len = udp.read(buffer, (sizeof(buffer) - 1));
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
        return ((rawspeed * 105) / 12 + ((rawspeed > 0) ? 150 : -150));
    }
}

int lastXspeed = 0;
int lastYspeed = 0;
void handleData(const char *buf) {
    int len;
    bool xchange = false, ychange = false;
    int xspeed = 0, yspeed = 0;

    if (buf[0] != '\0') {
        len = strlen(buf);
        for (int i = 0; i < len; i++) {
            if (buf[i] == 'X') {
                xspeed = lastXspeed = speedMapping(parseSpeed(&buf[(i + 2)]));
                xchange = true;
            }
            if (buf[i] == 'Y') {
                yspeed = lastYspeed = speedMapping(parseSpeed(&buf[(i + 2)]));
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

    if ((xspeed == 0) && (yspeed == 0)) {
        car.stop();
    } else if ((xspeed == 0) && (yspeed > 0)) {
        car.setAction(CAR_FORWARD, abs(yspeed));
    } else if ((xspeed == 0) && (yspeed < 0)) {
        car.setAction(CAR_BACKWARD, abs(yspeed));
    } else if ((xspeed > 0) && (yspeed < 0)) {
        car.setAction(CAR_BACKWARD_RIGHT, abs(yspeed));
    } else if ((xspeed > 0) && (yspeed == 0)) {
        car.setAction(CAR_ROTATE_RIGHT, abs(xspeed));
    } else if ((xspeed > 0) && (yspeed > 0)) {
        car.setAction(CAR_FORWARD_RIGHT, abs(yspeed));
    } else if ((xspeed < 0) && (yspeed < 0)) {
        car.setAction(CAR_BACKWARD_LEFT, abs(yspeed));
    } else if ((xspeed < 0) && (yspeed == 0)) {
        car.setAction(CAR_ROTATE_LEFT, abs(xspeed));
    } else if ((xspeed < 0) && (yspeed > 0)) {
        car.setAction(CAR_FORWARD_LEFT, abs(yspeed));
    }
}

int parseSpeed(const char *buf) {
    int s = 0;
    if (buf[0] == '-') {
        s = buf[1] - '0';
        if ((buf[2] >= '0') && (buf[2] <= '9')) {
            s = s * 10 + (buf[2] - '0');
        }
        s = -s;
    } else {
        s = buf[0] - '0';
        if ((buf[1] >= '0') && (buf[1] <= '9')) {
            s = s * 10 + (buf[1] - '0');
        }
    }
    return s;
}
