/*
 This example connects to a WEP-encrypted Wifi network.
 Then it prints the MAC address,
 the IP address obtained, and other network details.

 If you use 40-bit WEP, you need a key that is 10 characters long,
 and the characters must be hexadecimal (0-9 or A-F).
 e.g.  for 40-bit, ABBADEAF01 will work, but ABBADEAF won't work
 (too short) and ABBAISDEAF won't work (I and S are not
 hexadecimal characters).

 For 128-bit, you need a string that is 26 characters long.
 D0D0DEADF00DABBADEAFBEADED will work because it's 26 characters,
 all in the 0-9, A-F range.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe

 modified 08 May 2023
 by Realtek SG

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-connect-wifi/
 */

#include <WiFi.h>

// Set if user wants to key in ssid/pwd manually during operation
//#define MANUAL_INPUT 

#ifdef MANUAL_INPUT // initialise ssid string, pwd string, and serial_in object
// Initialise strings
String str_ssid, str_pass, str_key;
#endif

// 0: Exactly 10 or 26 hexadecimal characters; 1:Exactly 5 or 13 ASCII characters
#define password_type                           0

// If you are connecting to an iPhone WiFi hotspot, the default SSID uses Unicode (U+2019) Right Single Quotation Mark instead of ASCII apostrophe
// Modify the "Your Name" section in the SSID below to connect to an iPhone using a default SSID style
// char ssid[] = "Your Name\xE2\x80\x99s iPhone";

// UTF-8 encoding can also be used for SSID with emoji characters
// Emoji characters can be converted into UTF-8 at https://mothereff.in/utf-8
// char ssid[] = "\xe2\x9c\x8c\xef\xb8\x8f Ameba \xe2\x9c\x8c\xef\xb8\x8f";

char ssid[] = "Network_SSID";                   // your network SSID (name)
int keyIndex = 0;                               // your network key Index number
#if (password_type == 0)
char key[] = "D0D0DEADF00DABBADEAFBEADED";      // your network key, Exactly 10 or 26 hexadecimal characters
#elif (password_type == 1)
char pass[] = "D0D0D";                          // your network password, Exactly 5 or 13 ASCII characters
#else
    #error                                      // Error unsupported password type
#endif

int status = WL_IDLE_STATUS;                    // Indicator of Wifi status

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
#ifdef MANUAL_INPUT
        Serial.println("Enter your ssid");
        while (Serial.available() == 0) {}
        str_ssid = Serial.readString();
        str_ssid.trim();
        Serial.print("SSID entered: ");
        Serial.println(str_ssid);

        Serial.println("Enter your network key index number");
        while (Serial.available() == 0) {}
        str_key = Serial.readString();
        str_key.trim();
        Serial.print("Key entered: ");
        Serial.println(str_key);

        Serial.println("Enter your password");
        while (Serial.available() == 0) {}
        str_pass = Serial.readString();
        str_pass.trim();
            if (str_pass.length() != 0) { // user has entered data
                while (str_pass.length() < 5 ) { // to catch pwd<5 exception
                    Serial.println("Password cannot be less than 5 characters! Try again");
                    while (Serial.available() == 0) {}
                    str_pass = Serial.readString();
                    str_pass.trim();
                }
                    Serial.print("Password entered: ");
                    Serial.println(str_pass);
            }
#endif
        Serial.print("Attempting to connect to WEP network, SSID: ");
#ifndef MANUAL_INPUT
        Serial.println(ssid);
    #if (password_type == 0)
        status = WiFi.begin(ssid, keyIndex, key);
    #elif (password_type == 1)
        status = WiFi.begin(ssid, keyIndex, pass);
    #else
        #error      // Error unsupported password type
    #endif
#else
        char ssid_cust[str_ssid.length()+1];
        char key_cust[str_key.length()+1];
        char pass_cust[str_pass.length()+1];
        strcpy(ssid_cust, str_ssid.c_str());
        strcpy(key_cust, str_key.c_str());
        strcpy(pass_cust, str_pass.c_str());
        Serial.println(str_ssid.c_str());
        status = WiFi.begin(ssid_cust,atoi(key_cust), pass_cust);
        str_ssid = str_key = str_pass = "";
#endif
        // wait 10 seconds for connection:
        delay(10000);
    }

    // once you are connected :
    Serial.println();
    Serial.print("You're connected to the network");
    printCurrentNet();
    printWifiData();
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
    Serial.println(ip);

    // print your MAC address:
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    Serial.print(mac[0], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.println(mac[5], HEX);
}

void printCurrentNet() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print the MAC address of the router you're attached to:
    byte bssid[6];
    WiFi.BSSID(bssid);
    Serial.print("BSSID: ");
    Serial.print(bssid[5], HEX);
    Serial.print(":");
    Serial.print(bssid[4], HEX);
    Serial.print(":");
    Serial.print(bssid[3], HEX);
    Serial.print(":");
    Serial.print(bssid[2], HEX);
    Serial.print(":");
    Serial.print(bssid[1], HEX);
    Serial.print(":");
    Serial.println(bssid[0], HEX);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.println(rssi);

    // print the encryption type:
    byte encryption = WiFi.encryptionType();
    Serial.print("Encryption Type:");
    Serial.println(encryption, HEX);
    Serial.println();
}
