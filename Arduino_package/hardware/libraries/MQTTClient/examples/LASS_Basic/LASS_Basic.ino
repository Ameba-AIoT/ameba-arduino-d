/*
  This example demonstrate how to upload sensor data to MQTT server of LASS.
  It include features:
      (1) Connect to WiFi
      (2) Retrieve NTP time with WiFiUDP
      (3) Get PM 2.5 value from PMS3003 air condition sensor with UART
      (4) Connect to MQTT server and try reconnect when disconnect

  You can find more information at this site:

      https://lass.hackpad.com/LASS-README-DtZ5T6DXLbu

*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <PMS3003.h>

char ssid[] = "yourNetwork";    // your network SSID (name)
char pass[] = "secretPassword"; // your network password
int keyIndex = 0;               // your network key Index number (needed only for WEP)

char gps_lat[] = "24.7814033";  // device's gps latitude
char gps_lon[] = "120.9933676"; // device's gps longitude

char server[] = "gpssensor.ddns.net";      // the MQTT server of LASS
char clientId[17] = "";                    // client id for MQTT
char outTopic[20] = "LASS/Test/PM25/live"; // MQTT publish topic

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiUDP Udp;
const char ntpServer[] = "pool.ntp.org";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
const byte nptSendPacket[ NTP_PACKET_SIZE] = {
  0xE3, 0x00, 0x06, 0xEC, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x31, 0x4E, 0x31, 0x34,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
byte ntpRecvBuffer[ NTP_PACKET_SIZE ];

#define LEAP_YEAR(Y)     (((Y) > 0) && !((Y) % 4) && (((Y) % 100) || !((Y) % 400)))
static  const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // API starts months from 1, this array starts from 0
uint32_t epochSystem = 0; // timestamp of system boot up

#if defined(BOARD_RTL8195A)
PMS3003 pms(0, 1);  // SoftwareSerial RX/TX
#elif defined(BOARD_RTL8710)
PMS3003 pms(17, 5); // SoftwareSerial RX/TX
#else
PMS3003 pms(0, 1);  // SoftwareSerial RX/TX
#endif

void reconnectWiFi() {
    // attempt to connect to Wifi network:
    if ( WiFi.status() != WL_CONNECTED ) {
        Serial.print("Try connect to wifi ssid: ");
        Serial.println(ssid);
        while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
            delay(1000);
        }
        Serial.println("Connected to wifi");

        Udp.begin(2390); // local port to listen for UDP packets
    }
}

// send an NTP request to the time server at the given address
void retrieveNtpTime() {
    for (int retry = 0; retry < 5; retry++) {
        Serial.println("Send NTP packet");
        Udp.beginPacket(ntpServer, 123); //NTP requests are to port 123
        Udp.write(nptSendPacket, NTP_PACKET_SIZE);
        Udp.endPacket();

        delay(3000);
        if (Udp.parsePacket()) {
            Serial.println("NTP packet received");
            Udp.read(ntpRecvBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

            unsigned long highWord = word(ntpRecvBuffer[40], ntpRecvBuffer[41]);
            unsigned long lowWord = word(ntpRecvBuffer[42], ntpRecvBuffer[43]);
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            const unsigned long seventyYears = 2208988800UL;
            unsigned long epoch = secsSince1900 - seventyYears;

            epochSystem = epoch - millis() / 1000;
            break;
        }
    }
}

void getCurrentTime(unsigned long epoch, int *year, int *month, int *day, int *hour, int *minute, int *second) {
    int tempDay = 0;

    *hour = (epoch  % 86400L) / 3600;
    *minute = (epoch  % 3600) / 60;
    *second = epoch % 60;

    *year = 1970; // epoch starts from 1970
    *month = 0;
    *day = epoch / 86400;

    for (*year = 1970; ; (*year)++) {
        tempDay += (LEAP_YEAR(*year) ? 366 : 365);
        if (tempDay > *day) {
            tempDay -= (LEAP_YEAR(*year) ? 366 : 365);
            break;
        }
    }
    tempDay = *day - tempDay; // the days left in a year
    for ((*month) = 0; (*month) < 12; (*month)++) {
        if ((*month) == 1) {
            tempDay -= (LEAP_YEAR(*year) ? 29 : 28);
            if (tempDay < 0) {
                tempDay += (LEAP_YEAR(*year) ? 29 : 28);
                break;
            }
        } else {
            tempDay -= monthDays[(*month)];
            if (tempDay < 0) {
                tempDay += monthDays[(*month)];
                break;
            }
        }
    }
    *day = tempDay+1; // one for base 1, one for current day
    (*month)++;
}

void initializeMQTT() {
    byte mac[6];

    WiFi.macAddress(mac);
    sprintf(clientId, "FT_LIVE_%02X%02X%02X%02X", mac[2], mac[3], mac[4], mac[5]);

    Serial.print("MQTT client id:");
    Serial.println(clientId);
    Serial.print("MQTT topic:");
    Serial.println(outTopic);

    mqttClient.setServer(server, 1883);
}

void sendMQTT() {
    char payload[300];
    int pm25;
    unsigned long epoch = epochSystem + millis() / 1000;
    int year, month, day, hour, minute, second;
    getCurrentTime(epoch, &year, &month, &day, &hour, &minute, &second);

    pm25 = pms.get_pm2p5_air();
    Serial.print("pm2.5: ");
    Serial.print(pm25);
    Serial.println(" ug/m3");

    if (!mqttClient.connected()) {
        Serial.println("Attempting MQTT connection");
        mqttClient.connect(clientId);
    }

    if (mqttClient.connected()) {
        sprintf(payload, "|ver_format=3|FAKE_GPS=1|app=PM25|ver_app=%s|device_id=%s|date=%4d-%02d-%02d|time=%02d:%02d:%02d|s_d0=%d|gps_lon=%s|gps_lat=%s", "live", clientId, year, month, day, hour, minute, second, pm25, gps_lon, gps_lat);
        mqttClient.publish(outTopic, payload);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    pms.begin();
    reconnectWiFi();
    retrieveNtpTime();
    initializeMQTT();
}

void loop() {
    reconnectWiFi();
    sendMQTT();
    mqttClient.loop();
    delay(1000);
}
