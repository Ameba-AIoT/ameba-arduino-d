/*

 This example connects to a WEP-encrypted Wifi network.
 Then it prints the  MAC address of the Wifi shield,
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

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-connect-wifi/
 */

#include <WiFi.h>
#include "WifiSerial.h"

#define MANUAL_INPUT // Set if user wants to key in ssid/pwd manually during operation

#ifdef MANUAL_INPUT // initialise ssid string, pwd string, and serial_in object
// Initialise strings
std::string str_ssid, str_pass, str_key;
// Create serial_in object
WifiSerial wifiSerial;
#endif
// 0: Exactly 10 or 26 hexadecimal characters; 1:Exactly 5 or 13 ASCII characters
#define password_type                           0

// If you are connecting to an iPhone WiFi hotspot, the default SSID uses Unicode (U+2019) Right Single Quotation Mark instead of ASCII apostrophe
// Modify the "Your Name" section in the SSID below to connect to an iPhone using a default SSID style
// char ssid[] = "Your Name\xE2\x80\x99s iPhone";

// UTF-8 encoding can also be used for SSID with emoji characters
// Emoji characters can be converted into UTF-8 at https://mothereff.in/utf-8
// char ssid[] = "\xe2\x9c\x8c\xef\xb8\x8f Ameba \xe2\x9c\x8c\xef\xb8\x8f";

char ssid[] = "yourNetwork";                    // your network SSID (name)
int keyIndex = 0;                               // your network key Index number
#if (password_type == 0)
char key[] = "D0D0DEADF00DABBADEAFBEADED";      // your network key, Exactly 10 or 26 hexadecimal characters
#elif (password_type == 1)
char pass[] = "D0D0D";                          // your network password, Exactly 5 or 13 ASCII characters
#else
    #error                                      // Error unsupported password type
#endif

int status = WL_IDLE_STATUS;                    // the Wifi radio's status

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        #ifdef MANUAL_INPUT
        Serial.println("Enter your ssid");
        while (str_ssid.length() == 0) {
            str_ssid = wifiSerial.readInput();
            if (str_ssid.length() != 0) { // user has entered data
                Serial.print("SSID entered: ");
                Serial.println(str_ssid.c_str());
            }
        }

        Serial.println("Enter your network key index number");
        while (str_key.length() == 0) {
            str_key = wifiSerial.readInput();
                if (str_key.length() != 0) { // user has entered data
                    Serial.print("key entered: ");
                    Serial.println(str_key.c_str());
                }
        }

        Serial.println("Enter your password");
        while (str_pass.length() == 0) {
            str_pass = wifiSerial.readInput();
                if (str_pass.length() != 0) { // user has entered data
                    if (str_pass.length() <8){ // to catch password length < 8 exception
                        Serial.println("Password cannot be less than 8 characters! Try again");
                        str_pass = ""; // clear entered pwd and try again
                    }
                    Serial.print("Password entered: ");
                    Serial.println(str_pass.c_str());
                }
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
    #error                                       // Error unsupported password type
#endif
#endif

        #ifdef MANUAL_INPUT
        char ssid_cust[str_ssid.length()+1];
        char pass_cust[str_pass.length()+1];
        strcpy(ssid_cust, str_ssid.c_str());
        strcpy(pass_cust, str_pass.c_str());
        Serial.println(str_ssid.c_str());
        status = WiFi.begin(ssid_cust,std::stoi( str_key ), pass_cust);
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
    // print your WiFi shield's IP address:
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
