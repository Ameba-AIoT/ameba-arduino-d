/*
 This sketch shows how to read and open HTML file saved in SD card.

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-fatfs-sdio/
 */

// Core RTL8720DN do not support SDIO. Please use SPI interface for SD card
#ifndef CORE_RTL8720DN

#include <WiFi.h>
#include "FatFs_SD.h"

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password
int keyIndex = 0;                   // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

char filename_Web_test[] = "Web_test.html"; //name of the HTML file saved in SD card
FatFsSD fs;

void setup() {
    Serial.begin(115200);         // initialize serial communication
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);                   // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.setBlockingMode();
    server.begin();                           // start the web server on port 80
    printWifiStatus();                        // you're connected now, so print out the status
}

void loop() {
    char absolute_filename_html[128];
    
    WiFiClient client = server.available();     // listen for incoming clients

    if (client) {                               // if you get a client,
        Serial.println("new client");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
            if (client.available()) {           // if there's bytes to read from the client,
                char c = client.read();         // read a byte, then
                Serial.write(c);                // print it out the serial monitor
                if (c == '\n') {                // if the byte is a newline character

                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        fs.begin();

                        sprintf(absolute_filename_html, "%s%s", fs.getRootPath(), filename_Web_test);
                        SdFatFile file = fs.open(absolute_filename_html);

                        if(file){
                          while(file.available()){
                            client.write(file.read());
                          }
                          file.close();
                        }
                        fs.end();
                        // break out of the while loop:
                        break;
                    } else {    // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    // print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(ip);
}

#else

void setup() {}

void loop() {}

#endif
