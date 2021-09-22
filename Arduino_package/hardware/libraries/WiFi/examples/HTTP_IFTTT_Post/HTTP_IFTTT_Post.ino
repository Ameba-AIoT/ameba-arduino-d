#include <HttpClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

// This example downloads the URL "http://www.amebaiot.com"

char ssid[] = "network SSID"; // your network SSID (name)
char pass[] = "password";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key Index number (needed only for WEP)

// Name of the server we want to connect to
const char kHostname[] = "maker.ifttt.com";

const char kPath[] = "/trigger/test_event/with/key/<IFTTT Key>";
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;
int status = WL_IDLE_STATUS;

void setup() {
    Serial.begin(115200);
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();
}

void loop() {
    int err = 0;

    WiFiClient c;
    HttpClient http(c);

    err = http.get(kHostname, kPath);
    if (err == 0) {
        Serial.println("startedRequest ok");

        err = http.responseStatusCode();
        if (err >= 0) {
            Serial.print("Got status code: ");
            Serial.println(err);

            // Usually you'd check that the response code is 200 or a
            // similar "success" code (200-299) before carrying on,
            // but we'll print out whatever response we get

            err = http.skipResponseHeaders();
            if (err >= 0) {
                int bodyLen = http.contentLength();
                Serial.print("Content length is: ");
                Serial.println(bodyLen);
                Serial.println();
                Serial.println("Body returned follows:");

                // Now we've got to the body, so we can print it out
                unsigned long timeoutStart = millis();
                char c;
                // Whilst we haven't timed out & haven't reached the end of the body
                while ((http.connected() || http.available()) && ((millis() - timeoutStart) < kNetworkTimeout)) {
                    if (http.available()) {
                        c = http.read();
                        // Print out this character
                        Serial.print(c);
                        bodyLen--;
                        // We read something, reset the timeout counter
                        timeoutStart = millis();
                    } else {
                        // We haven't got any data, so let's pause to allow some to
                        // arrive
                        delay(kNetworkDelay);
                    }
                }
            } else {
                Serial.print("Failed to skip response headers: ");
                Serial.println(err);
            }
        } else {
            Serial.print("Getting response failed: ");
            Serial.println(err);
        }
    } else {
        Serial.print("Connect failed: ");
        Serial.println(err);
    }
    http.stop();

    // And just stop, now that we've tried a download
    while (1);
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
}
