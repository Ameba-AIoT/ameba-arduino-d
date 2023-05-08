/*
 This example connects to an unencrypted Wifi network.
 Then it prints the MAC address,
 the IP address obtained, and other network details.

 modified 10 Apr 2023
 by Realtek SG

 Example guide:
 
*/

#include <WiFi.h>

// Set if user wants to key in ssid/pwd manually during operation
// #define MANUAL_INPUT

#ifdef MANUAL_INPUT  // Initialise ssid string, pwd string, and serial_in object
// Initialise strings
String str_ssid, str_pass, str_apssid, str_appass, str_channel;
#endif

// If you are connecting to an iPhone WiFi hotspot, the default SSID uses Unicode (U+2019) Right Single Quotation Mark instead of ASCII apostrophe
// Modify the "Your Name" section in the SSID below to connect to an iPhone using a default SSID style
// char ssid[] = "Your Name\xE2\x80\x99s iPhone";

// UTF-8 encoding can also be used for SSID with emoji characters
// Emoji characters can be converted into UTF-8 at https://mothereff.in/utf-8
// char ssid[] = "\xe2\x9c\x8c\xef\xb8\x8f Ameba \xe2\x9c\x8c\xef\xb8\x8f";

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password

char apssid[] = "AP_Network_SSID";  // Set the AP SSID
char appass[] = "AP_Password";      // Set the AP password
char channel[] = "1";               // Set the AP channel
int status = WL_IDLE_STATUS;        // Indicater of Wifi status
int ssid_status = 0;                // Set SSID status, 1 hidden, 0 not hidden

void setup() {
    // Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    while (status != WL_CONNECTED) {
#ifdef MANUAL_INPUT
        Serial.println("Enter your ssid:");
        while (Serial.available() == 0) {}
        str_ssid = Serial.readString();
        str_ssid.trim();
        Serial.print("SSID entered: ");
        Serial.println(str_ssid);

        Serial.println("Enter your password:");
        while (Serial.available() == 0) {}
        str_pass = Serial.readString();
        str_pass.trim();
            if (str_pass.length() != 0) {
                while (str_pass.length() < 8) {
                    Serial.println("Password cannot be less than 8 characters! Try again");
                    while (Serial.available() == 0) {}
                    str_pass = Serial.readString();
                    str_pass.trim();
                }
                    Serial.print("Password entered: ");
                    Serial.println(str_pass);
            }

        Serial.println("Enter your AP ssid:");
        while (Serial.available() == 0) {}
        str_apssid = Serial.readString();
        str_apssid.trim();
        Serial.print("SSID entered: ");
        Serial.println(str_apssid);

        Serial.println("Enter your AP password:");
        while (Serial.available() == 0) {}
        str_appass = Serial.readString();
        str_appass.trim();
            if (str_appass.length() != 0) {
                while (str_appass.length() < 8) {
                    Serial.println("Password cannot be less than 8 characters! Try again");
                    while (Serial.available() == 0) {}
                    str_appass = Serial.readString();
                    str_appass.trim();
                }
                    Serial.print("Password entered: ");
                    Serial.println(str_appass);
            }
        Serial.println("Enter your channel number:");
        while (Serial.available() == 0) {}
            str_channel = Serial.readString();
            int checker = str_channel.toInt();
            while(str_channel != (String(checker))){
                Serial.println("channel should be a number!");
                while (Serial.available() == 0) {}
                str_channel = Serial.readString();
                checker = str_channel.toInt();
            }
            str_channel.trim();
            Serial.print("channel entered: ");
            Serial.println(str_channel);
#endif

#ifndef MANUAL_INPUT
        WiFi.enableConcurrent();
        status = WiFi.apbegin(apssid, appass, channel, ssid_status);
        delay(10000);
        status = WiFi.begin(ssid, pass);
#else
        char apssid_cust[str_apssid.length() + 1];
        char appass_cust[str_appass.length() + 1];
        char channel_cust[str_channel.length()+ 1];
        strcpy(apssid_cust, str_apssid.c_str());
        strcpy(appass_cust, str_appass.c_str());
        strcpy(channel_cust, str_channel.c_str());
        WiFi.enableConcurrent();
        status = WiFi.apbegin(apssid_cust, appass_cust, channel_cust, ssid_status);
        str_apssid = str_appass = str_channel = "";
        char ssid_cust[str_ssid.length() + 1];
        char pass_cust[str_pass.length() + 1];
        strcpy(ssid_cust, str_ssid.c_str());
        strcpy(pass_cust, str_pass.c_str());
        status = WiFi.begin(ssid_cust, pass_cust);
        str_ssid = str_pass = str_channel = "";
#endif
        delay(10000);
    }
    //AP mode started and connected to network
    //print out the data
    Serial.println("AP mode already started");
    printWifiData(1);
    Serial.println("Connected to the network");
    printWifiData(0);
    printCurrentNet();
}

void loop() {
  // put your main code here, to run repeatedly:
    delay(10000);
    printCurrentNet();
}

void printWifiData(int wlan_interface) {
    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP(wlan_interface);
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print your subnet mask:
    IPAddress subnet = WiFi.subnetMask(wlan_interface);
    Serial.print("NetMask: ");
    Serial.println(subnet);

    // print your gateway address:
    IPAddress gateway = WiFi.gatewayIP(wlan_interface);
    Serial.print("Gateway: ");
    Serial.println(gateway);
    Serial.println();
}

void printCurrentNet() {
    // print the SSID of the Network
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

    // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}
