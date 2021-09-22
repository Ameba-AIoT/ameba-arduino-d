#include <WiFi.h>

char ssid[] = "yourNetwork";  //Set the AP's SSID
char pass[] = "Password";     //Set the AP's password
char channel[] = "1";         //Set the AP's channel
int status = WL_IDLE_STATUS;  //Set the Wifi radio's status
int ssid_status = 0;          //Set SSID status, 1 hidden, 0 not hidden

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);
    }

    // attempt to start AP:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to start AP with SSID: ");
        Serial.println(ssid);
        status = WiFi.apbegin(ssid, pass, channel, ssid_status);
        delay(10000);
    }

    //AP MODE already started:
    Serial.println("AP mode already started");
    Serial.println();
    printWifiData();
    printCurrentNet();
}

void loop() {
    // check the network connection once every 10 seconds:
    delay(10000);
    printCurrentNet();
}

void printWifiData() {
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print your subnet mask:
    IPAddress subnet = WiFi.subnetMask();
    Serial.print("NetMask: ");
    Serial.println(subnet);

    // print your gateway address:
    IPAddress gateway = WiFi.gatewayIP();
    Serial.print("Gateway: ");
    Serial.println(gateway);
    Serial.println();
}

void printCurrentNet() {
    // print the SSID of the AP:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the MAC address of AP:
    byte bssid[6];
    WiFi.BSSID(bssid);
    Serial.print("BSSID: ");
    Serial.print(bssid[0], HEX);
    Serial.print(":");
    Serial.print(bssid[1], HEX);
    Serial.print(":");
    Serial.print(bssid[2], HEX);
    Serial.print(":");
    Serial.print(bssid[3], HEX);
    Serial.print(":");
    Serial.print(bssid[4], HEX);
    Serial.print(":");
    Serial.println(bssid[5], HEX);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}
