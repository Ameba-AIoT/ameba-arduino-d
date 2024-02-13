/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-ap-mode/
 */

#include <WiFi.h>
// Set if user wants to key in ssid/pwd manually during operation
//#define MANUAL_INPUT

#ifdef MANUAL_INPUT  // Initialise ssid string, pwd string, and serial_in object
// Initialise strings
String str_ssid, str_pass, str_channel;
#endif

// UTF-8 encoding can also be used for SSID with emoji characters
// Emoji characters can be converted into UTF-8 at https://mothereff.in/utf-8
// char ssid[] = "\xe2\x9c\x8c\xef\xb8\x8f Ameba \xe2\x9c\x8c\xef\xb8\x8f";

char ssid[] = "AP_Network_SSID";    // Set the AP SSID
char pass[] = "AP_Password";        // Set the AP password
char channel[] = "1";               // Set the AP channel
int status = WL_IDLE_STATUS;        // Indicator of Wifi status
int ssid_status = 0;                // Set SSID status, 1 hidden, 0 not hidden

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to start AP:
    while (status != WL_CONNECTED) {
#ifdef MANUAL_INPUT
        Serial.println("Enter your ssid");
        while (Serial.available() == 0) {}
            str_ssid = Serial.readString();
            str_ssid.trim();
            Serial.print("SSID entered: ");
            Serial.println(str_ssid);

        Serial.println("Enter your password");
        while (Serial.available() == 0) {}
        str_pass = Serial.readString();
        str_pass.trim();
            if (str_pass.length() != 0) { // user has entered data
                while (str_pass.length() <8 ) { // to catch pwd<8 exception
                    Serial.println("Password cannot be less than 8 characters! Try again");
                    while (Serial.available() == 0) {}
                    str_pass = Serial.readString();
                    str_pass.trim();
                }
                    Serial.print("Password entered: ");
                    Serial.println(str_pass);
            }

        Serial.println("Enter your channel number");
        while (Serial.available() == 0) {}
            str_channel = Serial.readString();
            str_channel.trim();
            int checker = str_channel.toInt();
            while(str_channel != (String(checker))){
                Serial.println("channel should be a number!");
                while (Serial.available() == 0) {}
                str_channel = Serial.readString();
                checker = str_channel.toInt();
            }
            Serial.print("channel entered: ");
            Serial.println(str_channel);
#endif
        Serial.print("Attempting to start AP with SSID: ");
#ifndef MANUAL_INPUT
        Serial.println(ssid);
        status = WiFi.apbegin(ssid, pass, channel, ssid_status);
#else
        char ssid_cust[str_ssid.length() + 1];
        char pass_cust[str_pass.length() + 1];
        char channel_cust[str_channel.length() + 1];
        strcpy(ssid_cust, str_ssid.c_str());
        strcpy(pass_cust, str_pass.c_str());
        strcpy(channel_cust, str_channel.c_str());
        Serial.println(str_ssid.c_str());
        status = WiFi.apbegin(ssid_cust, pass_cust, channel_cust, ssid_status);
        str_ssid = str_pass = str_channel = "";
#endif
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
    // print your WiFi IP address:
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
