/*
 *  This sketch shows how to use Car2wd class to control L298N on 2WD car
 *  and control it using web interface over WiFi
 *
 *  Library needed: 
 *      AmebaMotor.zip  (download at https://github.com/ambiot/ambd_arduino/blob/master/Arduino_zip_libraries/AmebaMotors.zip)
 *      PS: You may add this zip library using Arduino IDE directly
 *
 *  Demo Video Available at:
 *      https://youtu.be/ItVaPQ4dv8Q
 *
 **/

#include "Car2wd.h"
#include <WiFi.h>

char ssid[] = "YourWiFiSSID";        //  your network SSID (name)
char pass[] = "WiFiPassword";        // your network password
int keyIndex = 0;                    // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

Car2wd car(8,9,10,11,12,13);         // Pins needed for L298N module

void setup() {
    car.begin();
    Serial.begin(115200);            // initialize serial communication

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);                // don't continue
    }

    String fv = WiFi.firmwareVersion();
    if (fv != "1.0.0") {
        Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid);         // print the network name (SSID);

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    server.begin();                             // start the web server on port 80
    printWifiStatus();                          // you're connected now, so print out the status
}

void loop() {
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

                        // the content of the HTTP response follows the header:
                        client.print("Click <a href=\"/U\">here</a> to move Forward<br>");
                        client.print("Click <a href=\"/D\">here</a> to move Backward<br>");
                        client.print("Click <a href=\"/R\">here</a> to turn Right<br>");
                        client.print("Click <a href=\"/L\">here</a> to turn Left<br>");

                        // The HTTP response ends with another blank line:
                        client.println();
                        // break out of the while loop:
                        break;
                    } else {                // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                } else if (c != '\r') {     // if you got anything else but a carriage return character,
                    currentLine += c;       // add it to the end of the currentLine
                }

                // Check client request 
                if (currentLine.endsWith("GET /U")) {
                    car.setAction(CAR_FORWARD);
                    delay(2000);
                    car.setAction(CAR_STOP);
                    delay(1000);
                }
                if (currentLine.endsWith("GET /D")) {
                    car.setAction(CAR_BACKWARD);
                    delay(2000);
                    car.setAction(CAR_STOP);
                    delay(1000);
                }
                if (currentLine.endsWith("GET /R")) {
                    car.setAction(CAR_FORWARD_RIGHT);
                    delay(2000);
                    car.setAction(CAR_STOP);
                    delay(1000);               
                }
                if (currentLine.endsWith("GET /L")) {
                    car.setAction(CAR_FORWARD_LEFT);
                    delay(2000);
                    car.setAction(CAR_STOP);
                    delay(1000);
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disonnected");
    }
  
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
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
