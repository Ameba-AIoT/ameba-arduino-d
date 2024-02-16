/*
 This sketch provide a simple way to roughly calculate the delay of Ameba receive delay.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-udp-receive-delay/
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
    Udp.begin(localPort);
}

char buf[256];
void loop() {
    int n;
    while (1) {
        memset(buf, 0, 256);
        n = Udp.read(buf, 255);
        if (n > 0) {
            handle_data(buf);
        }
    }
}

long early_diff = 0;

long ameba_epoch = 0;
long long sys_epoch = 0;

long datacount = 0;
long total_shift = 0;

void handle_data(char *buf) {
    long data;
    long timediff;
    long current_epoch;

    data = atol(buf);
    if (ameba_epoch == 0) {
        /* We sync Ameba's system and PC's system time on first packet */
        ameba_epoch = millis();
        sys_epoch = data;
    } else {
        current_epoch = millis();
        timediff = (current_epoch - ameba_epoch) - (data - sys_epoch);

        if (-timediff > early_diff) {
            /* This packet came in earlier than expected. It means there is some delay at first packet. Record this deley. */
            early_diff = -timediff;
        }

        total_shift += timediff;
        datacount++;

        if (datacount % 10000 == 0) {
            Serial.print("data count: ");
            Serial.print(datacount);
            Serial.print("\taverage delay: ");
            Serial.print(early_diff + total_shift * 1.0 / datacount);
            Serial.println(" ms");
        }
    }
}
