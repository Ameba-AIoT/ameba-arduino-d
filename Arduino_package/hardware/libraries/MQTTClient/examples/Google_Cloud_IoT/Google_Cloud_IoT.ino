/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-googlecloud/
 */

#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include "src/jwt.h"

#define count   100

WiFiSSLClient wifiClient;
PubSubClient client(wifiClient);
WiFiUDP udpClient;
NTPClient timeClient(udpClient);

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)

char GOOGLE_MQTT_SERVER[] = "mqtt.googleapis.com";
int  GOOGLE_MQTT_PORT = 8883;

char project_id[] = "project-id";
char location[] = "us-central1";
char registry_id[] = "my-registry";
char device_id[] = "amebago-device";

char clientUser[] = "unused";
String clientPass;
char *mqtt_id;
char *pub_topic;
char payload[64];
NN_DIGIT priv_key[9];

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length below (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something
// is probably wrong with your key.
char *private_key_str =
"57:e1:c0:3b:e5:cd:2f:42:fb:14:4f:a9:1e:3f:0a:"
"65:22:4c:f0:ac:0f:a0:82:e1:73:71:ae:76:70:48:"
"68:fe";

// To get the certificate for your region run:
//   openssl s_client -showcerts -connect mqtt.googleapis.com:8883
// for standard mqtt or for LTS:
//   openssl s_client -showcerts -connect mqtt.2030.ltsapis.goog:8883
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
char* rootCABuff  = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDvDCCAqSgAwIBAgINAgPk9GHsmdnVeWbKejANBgkqhkiG9w0BAQUFADBMMSAw\n" \
"HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEGA1UEChMKR2xvYmFs\n" \
"U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0wNjEyMTUwODAwMDBaFw0yMTEy\n" \
"MTUwODAwMDBaMEwxIDAeBgNVBAsTF0dsb2JhbFNpZ24gUm9vdCBDQSAtIFIyMRMw\n" \
"EQYDVQQKEwpHbG9iYWxTaWduMRMwEQYDVQQDEwpHbG9iYWxTaWduMIIBIjANBgkq\n" \
"hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAps8kDr4ubyiZRULEqz4hVJsL03+EcPoS\n" \
"s8u/h1/Gf4bTsjBc1v2t8Xvc5fhglgmSEPXQU977e35ziKxSiHtKpspJpl6op4xa\n" \
"Ebx6guu+jOmzrJYlB5dKmSoHL7Qed7+KD7UCfBuWuMW5Oiy81hK561l94tAGhl9e\n" \
"SWq1OV6INOy8eAwImIRsqM1LtKB9DHlN8LgtyyHK1WxbfeGgKYSh+dOUScskYpEg\n" \
"vN0L1dnM+eonCitzkcadG6zIy+jgoPQvkItN+7A2G/YZeoXgbfJhE4hcn+CTClGX\n" \
"ilrOr6vV96oJqmC93Nlf33KpYBNeAAHJSvo/pOoHAyECjoLKA8KbjwIDAQABo4Gc\n" \
"MIGZMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBSb\n" \
"4gdXZxwewGoG3lm0mi3f3BmGLjAfBgNVHSMEGDAWgBSb4gdXZxwewGoG3lm0mi3f\n" \
"3BmGLjA2BgNVHR8ELzAtMCugKaAnhiVodHRwOi8vY3JsLmdsb2JhbHNpZ24ubmV0\n" \
"L3Jvb3QtcjIuY3JsMA0GCSqGSIb3DQEBBQUAA4IBAQANeX81Z1YqDIs4EaLjG0qP\n" \
"OxIzaJI/y4kiRj3a+y3KOx74clIkLuMgi/9/5iv/n+1LyhGU9g7174slbzJOPbSp\n" \
"p1eT19ST2mYbdgTLx/hm3tTLoHIY/w4ZbnQYwfnPwAG4RefnEFYPQJmpD+Wh8BJw\n" \
"Bgtm2drTale/T6NBwmwnEFunfaMfMX3g6IBrx7VKnxIkJh/3p190WveLKgl9n7i5\n" \
"SWce/4woPimEn9WfEQWRvp6wKhaCKFjuCMuulEZusoOUJ4LfJnXxcuQTgIrSnwI7\n" \
"KfSSjsd42w3lX1fbgJp7vPmLM6OBRvAXuYRKTFqMAWbb7OaGIEE+cbxY6PDepnva\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
"27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
"Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
"TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
"qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
"szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
"Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
"MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
"wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
"aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
"VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
"C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
"QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
"h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
"7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
"ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
"MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
"Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
"6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
"0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
"2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
"bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlrsWNBCUaqxElqjANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjIwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjIwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQDO3v2m++zsFDQ8BwZabFn3GTXd98GdVarTzTukk3LvCvpt\n" \
"nfbwhYBboUhSnznFt+4orO/LdmgUud+tAWyZH8QiHZ/+cnfgLFuv5AS/T3KgGjSY\n" \
"6Dlo7JUle3ah5mm5hRm9iYz+re026nO8/4Piy33B0s5Ks40FnotJk9/BW9BuXvAu\n" \
"MC6C/Pq8tBcKSOWIm8Wba96wyrQD8Nr0kLhlZPdcTK3ofmZemde4wj7I0BOdre7k\n" \
"RXuJVfeKH2JShBKzwkCX44ofR5GmdFrS+LFjKBC4swm4VndAoiaYecb+3yXuPuWg\n" \
"f9RhD1FLPD+M2uFwdNjCaKH5wQzpoeJ/u1U8dgbuak7MkogwTZq9TwtImoS1mKPV\n" \
"+3PBV2HdKFZ1E66HjucMUQkQdYhMvI35ezzUIkgfKtzra7tEscszcTJGr61K8Yzo\n" \
"dDqs5xoic4DSMPclQsciOzsSrZYuxsN2B6ogtzVJV+mSSeh2FnIxZyuWfoqjx5RW\n" \
"Ir9qS34BIbIjMt/kmkRtWVtd9QCgHJvGeJeNkP+byKq0rxFROV7Z+2et1VsRnTKa\n" \
"G73VululycslaVNVJ1zgyjbLiGH7HrfQy+4W+9OmTN6SpdTi3/UGVN4unUu0kzCq\n" \
"gc7dGtxRcw1PcOnlthYhGXmy5okLdWTK1au8CcEYof/UVKGFPP0UJAOyh9OktwID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQUu//KjiOfT5nK2+JopqUVJxce2Q4wDQYJKoZIhvcNAQEMBQADggIBAB/Kzt3H\n" \
"vqGf2SdMC9wXmBFqiN495nFWcrKeGk6c1SuYJF2ba3uwM4IJvd8lRuqYnrYb/oM8\n" \
"0mJhwQTtzuDFycgTE1XnqGOtjHsB/ncw4c5omwX4Eu55MaBBRTUoCnGkJE+M3DyC\n" \
"B19m3H0Q/gxhswWV7uGugQ+o+MePTagjAiZrHYNSVc61LwDKgEDg4XSsYPWHgJ2u\n" \
"NmSRXbBoGOqKYcl3qJfEycel/FVL8/B/uWU9J2jQzGv6U53hkRrJXRqWbTKH7QMg\n" \
"yALOWr7Z6v2yTcQvG99fevX4i8buMTolUVVnjWQye+mew4K6Ki3pHrTgSAai/Gev\n" \
"HyICc/sgCq+dVEuhzf9gR7A/Xe8bVr2XIZYtCtFenTgCR2y59PYjJbigapordwj6\n" \
"xLEokCZYCDzifqrXPW+6MYgKBesntaFJ7qBFVHvmJ2WZICGoo7z7GJa7Um8M7YNR\n" \
"TOlZ4iBgxcJlkoKM8xAfDoqXvneCbT+PHV28SSe9zE8P4c52hgQjxcCMElv924Sg\n" \
"JPFI/2R80L5cFtHvma3AH/vLrrw4IgYmZNralw4/KBVEqE8AyvCazM90arQ+POuV\n" \
"7LXTWtiBmelDGDfrs7vRWGJB82bSj6p4lVQgw1oudCvV0b4YacCs1aTPObpRhANl\n" \
"6WLAYv7YTVWW4tAR+kg0Eeye7QUd5MjWHYbL\n" \
"-----END CERTIFICATE-----\n" ;

void fillPrivateKey(const char *private_key) {
    priv_key[8] = 0;
    for (int i = 7; i >= 0; i--) {
        priv_key[i] = 0;
        for (int byte_num = 0; byte_num < 4; byte_num++) {
            priv_key[i] = (priv_key[i] << 8) + strtoul(private_key, NULL, 16);
            private_key += 3;
        }
    }
}

void setup() {
    Serial.begin(115200);
    // attempt to connect to Wifi network:
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
        delay(1000);
    }
    Serial.println("Connected to wifi");

    fillPrivateKey(private_key_str);
    timeClient.begin();
    wifiClient.setRootCA((unsigned char*)rootCABuff);
}

void loop() {
    boolean ret = 0;
    if (WiFi.status() != WL_CONNECTED) {
        while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
            delay(1000);
        }
        Serial.println("Connected to wifi");
    }

    if(ret == 0) {
        Serial.println("Ready for Publishing");

        mqtt_id = (char *)malloc(strlen("projects/") + strlen(project_id) + strlen("/locations/") + strlen(location) + strlen("/registries/") + strlen(registry_id) + strlen("/devices/") + strlen(device_id) + 1);
        sprintf(mqtt_id, "projects/%s/locations/%s/registries/%s/devices/%s", project_id, location, registry_id, device_id);

        pub_topic = (char *)malloc(strlen("/devices/") + strlen(device_id) + strlen("/events") + 1);
        sprintf(pub_topic, "/devices/%s/events", device_id);

        clientPass = CreateJwt(project_id, timeClient.getEpochTime(), priv_key);

        client.setServer(GOOGLE_MQTT_SERVER, GOOGLE_MQTT_PORT);
        client.setPublishQos(MQTTQOS1);
        client.waitForAck(true);
    }

    Serial.println("client connecting...");
    if (client.connect(mqtt_id, clientUser, clientPass.c_str())) {
        for(int i = 0; i < count; i++) {
            memset(payload, 0x0, 64);
            sprintf(payload, "This is Ameba's %d message!!", i);
            printf("Publishing the payload \"%s\" with len: %d\r\n", payload, strlen(payload));
            ret = client.publish(pub_topic, payload);
            if (ret == 1) {
                printf("client publish successfully!!! ret = %d\r\n",ret);
            } else {
                printf("client publish failed!!! ret = %d\r\n",ret);
                break;
            }
            if (!client.connected()) {
                Serial.println("MQTT disconnected from server");
                break;
            }

            delay(1000);
        }
        client.disconnect();
    }

    free(mqtt_id);
    free(pub_topic);

    timeClient.update();
}
