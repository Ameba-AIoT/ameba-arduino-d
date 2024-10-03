#include "OTA.h"
#include "WiFi.h"

char ssid[] = "Network_SSID5";    // your network SSID (name)
char pass[] = "Password";         // your network password
int status = WL_IDLE_STATUS;

int PORT = 3000;                  // your server port number
char *SERVER = "192.168.3.14";    // your server ip address

class OTA ota;

void setup()
{
    Serial.begin(115200);

    // Connection to internet
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(2000);
    }

    // Set up the threads
    ota.start_OTA_threads(PORT, SERVER);
}

void loop()
{
    // Empty or add non-blocking code here
}
