/*
 This sketch provide a simple way to roughly calculate the delay of Ameba send delay.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-udp-send-delay/
 */

#include <WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;        // Indicator of Wifi status

WiFiUDP Udp;
char client_ip[] ="0.0.0.0";
String str_client_ip;
int port = 5001;

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

    Serial.print("Enter client IP address, ");
    while (Serial.available() == 0);
    str_client_ip = Serial.readString();
    str_client_ip.trim();
    str_client_ip.toCharArray(client_ip, (str_client_ip.length() + 1));
    Serial.println(client_ip);
    delay(5000);
}

char buf[256];
void loop() {
    sprintf(buf, "%d", ((int)(millis())));
    Udp.beginPacket(client_ip, port);
    Udp.write(buf);
    Udp.endPacket();
    delay(5);
}
