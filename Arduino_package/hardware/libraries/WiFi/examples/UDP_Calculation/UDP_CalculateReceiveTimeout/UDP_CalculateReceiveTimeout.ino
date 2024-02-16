/*
 This sketch provide a way to measure proper udp timeout value via dynamically changing udp
 receiving timeout value. If timeout happens, then add 1 to receiving timeout. Otherwise
 decrease receiving timeout.

 Please open Serial Plotter to check the change behavior of timeout value.
 The meaning of timeout value depends on the sending frequency from sender side.
 If the sender side send packets frequently, then the Arduino side can have smaller receiving timeout value.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-udp-timeout/
 */

#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdio.h>

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;        // Indicator of Wifi status

unsigned int localPort = 5001;  // local port to listen for UDP packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
IPAddress ip;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect.
    }

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        if (status == WL_CONNECTED) {
            break;
        }
        // wait 10 seconds for connection:
        delay(10000);
    }

    Serial.println("\n\rConnected to wifi");
    ip = WiFi.localIP();

    Udp.begin(localPort);
}

int timeout = 1000;
char buf[256];
void loop() {
    int n;
    while (1) {
        memset(buf, 0, 256);
        n = Udp.read(buf, 255);

        /* You can change different penalty way here. (Ex. two continuous timeouts add 10 to timeout value) */
        if (n <= 0) {
            timeout++;
            if (timeout > 3000) {
                // assume that the udp timeout is no more than 3s
                timeout = 3000;
            }
        } else {
            timeout--;
            if (timeout < 1) {
                timeout = 1;
            }
        }

        Serial.print("IP Address: ");
        Serial.print(ip);
        Serial.print("    Timeout: ");
        Serial.println(timeout);
        Udp.setRecvTimeout(timeout);
    }
}
