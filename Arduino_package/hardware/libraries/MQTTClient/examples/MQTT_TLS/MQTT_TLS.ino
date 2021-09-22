#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
char ssid[] = "yourNetwork";        // your network SSID (name)
char pass[] = "yourPassword";     	// your network password (use for WPA, or use as key for WEP)
int status  = WL_IDLE_STATUS;    	// the Wifi radio's status

char mqttServer[]     = "test.mosquitto.org";
char clientId[]       = "amebaClient";
char publishTopic[]   = "outTopic";
char publishPayload[] = "hello world";
char subscribeTopic[] = "inTopic";

WiFiSSLClient wifiClient;
PubSubClient client(wifiClient);

/* Mosquitto Root CA can be download here:
 * https://test.mosquitto.org/
 */
char* rootCABuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEAzCCAuugAwIBAgIUBY1hlCGvdj4NhBXkZ/uLUZNILAwwDQYJKoZIhvcNAQEL\n" \
"BQAwgZAxCzAJBgNVBAYTAkdCMRcwFQYDVQQIDA5Vbml0ZWQgS2luZ2RvbTEOMAwG\n" \
"A1UEBwwFRGVyYnkxEjAQBgNVBAoMCU1vc3F1aXR0bzELMAkGA1UECwwCQ0ExFjAU\n" \
"BgNVBAMMDW1vc3F1aXR0by5vcmcxHzAdBgkqhkiG9w0BCQEWEHJvZ2VyQGF0Y2hv\n" \
"by5vcmcwHhcNMjAwNjA5MTEwNjM5WhcNMzAwNjA3MTEwNjM5WjCBkDELMAkGA1UE\n" \
"BhMCR0IxFzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTES\n" \
"MBAGA1UECgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVp\n" \
"dHRvLm9yZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzCCASIwDQYJ\n" \
"KoZIhvcNAQEBBQADggEPADCCAQoCggEBAME0HKmIzfTOwkKLT3THHe+ObdizamPg\n" \
"UZmD64Tf3zJdNeYGYn4CEXbyP6fy3tWc8S2boW6dzrH8SdFf9uo320GJA9B7U1FW\n" \
"Te3xda/Lm3JFfaHjkWw7jBwcauQZjpGINHapHRlpiCZsquAthOgxW9SgDgYlGzEA\n" \
"s06pkEFiMw+qDfLo/sxFKB6vQlFekMeCymjLCbNwPJyqyhFmPWwio/PDMruBTzPH\n" \
"3cioBnrJWKXc3OjXdLGFJOfj7pP0j/dr2LH72eSvv3PQQFl90CZPFhrCUcRHSSxo\n" \
"E6yjGOdnz7f6PveLIB574kQORwt8ePn0yidrTC1ictikED3nHYhMUOUCAwEAAaNT\n" \
"MFEwHQYDVR0OBBYEFPVV6xBUFPiGKDyo5V3+Hbh4N9YSMB8GA1UdIwQYMBaAFPVV\n" \
"6xBUFPiGKDyo5V3+Hbh4N9YSMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL\n" \
"BQADggEBAGa9kS21N70ThM6/Hj9D7mbVxKLBjVWe2TPsGfbl3rEDfZ+OKRZ2j6AC\n" \
"6r7jb4TZO3dzF2p6dgbrlU71Y/4K0TdzIjRj3cQ3KSm41JvUQ0hZ/c04iGDg/xWf\n" \
"+pp58nfPAYwuerruPNWmlStWAXf0UTqRtg4hQDWBuUFDJTuWuuBvEXudz74eh/wK\n" \
"sMwfu1HFvjy5Z0iMDU8PUDepjVolOCue9ashlS4EB5IECdSR2TItnAIiIwimx839\n" \
"LdUdRudafMu5T5Xma182OC0/u/xRlEm+tvKGGmfFcN0piqVl8OrSPBgIlb+1IKJE\n" \
"m/XriWr/Cq4h/JfB7NTsezVslgkBaoU=\n" \
"-----END CERTIFICATE-----\n";

char* certificateBuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDjTCCAnWgAwIBAgIBADANBgkqhkiG9w0BAQsFADCBkDELMAkGA1UEBhMCR0Ix\n" \
"FzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTESMBAGA1UE\n" \
"CgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVpdHRvLm9y\n" \
"ZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzAeFw0yMDA3MTcxMDM0\n" \
"MjRaFw0yMDEwMTUxMDM0MjRaMGcxCzAJBgNVBAYTAlNHMRIwEAYDVQQIDAlTaW5n\n" \
"YXBvcmUxEjAQBgNVBAcMCVNpbmdhcG9yZTEQMA4GA1UECgwHUmVhbHRlazEMMAoG\n" \
"A1UECwwDU0QzMRAwDgYDVQQDDAdhdXJpY2FsMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
"AQ8AMIIBCgKCAQEAmog1ck7TyKdDw/943tfGLvwY/rz6DCZkosOQCYEZhjMSjX4M\n" \
"BNcCU/rhwm8EP+1uYZfHrdsijalMuSUGUOi9Y1t+SqzYLvvgLi8+h3LIGiGvqqhs\n" \
"97iyWNu1WfWEMqBxjRVAI7/+u3b/1Ztkz9UBN7+/y4jGLTsaW4IRQ5qCwt58ov+Q\n" \
"QU2PYS8qMlCRTxaokXOiLnBkxsOTq+aKLtDlx8nHjXSNCnOPjzzc4pq7rMjGyzq5\n" \
"edrO/pX9yFjD+wTGyssdnhTBgL8xTsx7mfDfpgdT/Bk7wZWQyfLosssdYLGqU6MN\n" \
"8wI13wmqBymHAeFkihMBf4kDlgkWLYZuziBWSwIDAQABoxowGDAJBgNVHRMEAjAA\n" \
"MAsGA1UdDwQEAwIF4DANBgkqhkiG9w0BAQsFAAOCAQEASqd/fRlpuUgehZMjzRNO\n" \
"RADV/gjX/UkWhlu66HbWhPDTuu5Nesms79bR7IbnP0umdtt8nYdrW/ersTTFYkEv\n" \
"7rV6xlATpjXhuz1bwYeRSg1cG+3cjiyFmMr2tlaVHMpXzfHBbEwOvbqIonPQnEe9\n" \
"6X0j/10xKMdsvKCb9OpAqlfDd4199M6tlTJiqaZ6OVAGpJ/ga0LNu0MNHPu8s68X\n" \
"ODZA8GwI01kQUxUkrmaYmth3R4A3uIYfs4ff9u71TgT9wtnwQFtacHhVAmF+hQpW\n" \
"luJqKtHI/Ox0Mya0Y1aONZavT3WgtxnRmEdAv6gn9WNfko6qr98AqGijX2VZpjRf\n" \
"oQ==\n" \
"-----END CERTIFICATE-----\n";

char* privateKeyBuff = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAmog1ck7TyKdDw/943tfGLvwY/rz6DCZkosOQCYEZhjMSjX4M\n" \
"BNcCU/rhwm8EP+1uYZfHrdsijalMuSUGUOi9Y1t+SqzYLvvgLi8+h3LIGiGvqqhs\n" \
"97iyWNu1WfWEMqBxjRVAI7/+u3b/1Ztkz9UBN7+/y4jGLTsaW4IRQ5qCwt58ov+Q\n" \
"QU2PYS8qMlCRTxaokXOiLnBkxsOTq+aKLtDlx8nHjXSNCnOPjzzc4pq7rMjGyzq5\n" \
"edrO/pX9yFjD+wTGyssdnhTBgL8xTsx7mfDfpgdT/Bk7wZWQyfLosssdYLGqU6MN\n" \
"8wI13wmqBymHAeFkihMBf4kDlgkWLYZuziBWSwIDAQABAoIBAQCWS3uP3VLliKjD\n" \
"66inM70w3UeCGlInlv+xvfpNBB7LAsk8jCcsXeGJxCvmawr7EfWVPYWbddG5bbuN\n" \
"JpFqB7KK++Vsw4bOy3JR+Okb22r632B23rB8jkoHWkibYcpdYuaNjOmA267orOhN\n" \
"3IDEEirJCdmEH1Z/j0gnT+1yjsg2taIl7v6sObapY6qTrxRI2lzf2zXQbyHXTvnV\n" \
"G9kVaa3fpk7czsDUZaKI/Pu+alGqJwQEYIB1Tbj7k65aZsixIdkA7/mexk2OTVM9\n" \
"oRmgf1OGn3+RHWbu9mfbH+b/acE94LeV0hBmQxope7gFW5SGorQ8Y3D+TEUCK64m\n" \
"nQPgAkPhAoGBAMdiXVrvhaL6SR1Zmjbb6XZeQsOi2icyP0gLOowIgRI6JZ01uslL\n" \
"2C2ZaiZvGFHeM62PNDTsqVNv//5gkAqHDxV4KwvuPYjbfqJI+cPy8Lj6NN4kJ0Se\n" \
"Jssuo5QcEOXaN9CkfDokH8L2shr8XNUExJLAwkDVUxzB0UNkahOfAgRNAoGBAMZp\n" \
"c2x9nxsnzQ2CAF26PRgBMgkOLXhF14Zk3sQqQLY6lUdkaSR+NR2PVCGsVw2GtCTe\n" \
"QBvAJL/xDBoMoA0CPVXNxxTTGrGtxXHBi1vEyCGiilGImE4Q6445O39TO/UUsaOd\n" \
"R8zqMpCLr6N5Dp6a6KDCQ5sMkranz5c01OuGvvD3AoGAfcKccbQsjXziyyFuNPYG\n" \
"zsNrxywb/6cg5Rc4CYV/q/iDKyLtuvQc2b4vhyiBpXnLDMkLu+oeb2Nwi4lqsUk5\n" \
"CWivdioCJCWWPVRcPe0pSvN+jzMslYpPm87CHLSLL/+xPufQ93c7lWS3OHA/OZl1\n" \
"R79Ph0dG0heqH6Dhi5i60+0CgYEAk2xQ5gClErFNC8dXbEzwDgD9Y3sUT/RdAq+6\n" \
"UbkvYfc/ac9yzDm2T6NqmKaL/DHlcittKPFonPGG02gkF+ZdGn8OeCuu/uwl87Mi\n" \
"T+pz5YH3EkPTHtY1n/Rof9+hgiWBJBe9/MTbE0mnFGjjcveY97HAN9lsvakMdsY7\n" \
"mlEBWNsCgYBAsNfSFgDnSTcsSkfx/dYSXaQmKuoEDCztWjx+lDii3nvkI1HigM4P\n" \
"Q0fJ6v8y/ivow9zRYuO+k8VN5TRe7ml3VsKYk6rCX80MuKR9oOPvwyuoJ6Bi/51h\n" \
"6keVd5li5TtenbNLgDZxKTelJtDO8zSf1Fix/UnbN1kRM8ka+yAflw==\n" \
"-----END RSA PRIVATE KEY-----\n";

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)(payload[i]));
    }
    Serial.println();
}

void reconnect() {
    // Loop until we're reconnected
    while (!(client.connected())) {
        Serial.println("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(clientId)) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish(publishTopic, publishPayload);
            // ... and resubscribe
            client.subscribe(subscribeTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
  Serial.begin(115200);

  while (status != WL_CONNECTED) {
    Serial.print("\r\n Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) break;
    // retry after 1 second
    delay(1000);
  }

  /*/
  wifiClient.setRootCA((unsigned char*)rootCABuff);
  wifiClient.setClientCertificate((unsigned char*)certificateBuff, (unsigned char*)privateKeyBuff);
  client.setServer(mqttServer, 8884);
  /*/
  wifiClient.setRootCA((unsigned char*)rootCABuff);
  client.setServer(mqttServer, 8883);
  //*/
  client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
}