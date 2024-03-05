/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-web-server-status/
 */

#include <WiFi.h>

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;        // Indicator of Wifi status

//State the analog pin that you want to read
#ifdef BOARD_AITHINKER_BW16
int analogChannel = A2;  // on the BW16 only A2 is available
#elif BOARD_AMB25 || BOARD_AMB26
int analogChannel = A4;  // on the AMB25/AMB26 only A4 A5 A6 are available
#else
int analogChannel = A0;  // Default using A0
#endif

WiFiServer server(80);
void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    // server.setBlocking();
    server.begin();
    // you're connected now, so print out the status:
    printWifiStatus();
}

void loop() {
    // listen for incoming clients
    WiFiClient client = server.available();
    if (client) {
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        Serial.println("new client");
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    // output the value of each analog input pin
                    int sensorReading = analogRead(analogChannel);
                    client.print("analog input reading is ");
                    client.print(sensorReading);
                    client.println("<br />");
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                  // you're starting a new line
                  currentLineIsBlank = true;
                } else if (c != '\r') {
                  // you've gotten a character on the current line
                  currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);

        // close the connection:
        // client.stop(); // remove this line since destructor will be called automatically
        Serial.println("client disconnected");
    }
    // continue with user code in WiFi server non-blocking mode 
    Serial.println("User code implementing here...");
    delay(5000);
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
