/*
 Basic Amazon AWS IoT example
*/

#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
char ssid[] = "yourNetwork";        // your network SSID (name)
char pass[] = "yourPassword";     	// your network password (use for WPA, or use as key for WEP)
int status  = WL_IDLE_STATUS;    	// the Wifi radio's status

WiFiSSLClient wifiClient;
PubSubClient client(wifiClient);

#define THING_NAME "amebaDevBoard"

char mqttServer[]     = "a4adrtgyzxov42-ats.iot.ap-southeast-1.amazonaws.com";
char clientId[]       = "amebaClient";
char publishTopic[]   = "$aws/things/" THING_NAME "/shadow/update";
char publishPayload[MQTT_MAX_PACKET_SIZE];
char *subscribeTopic[5] = {
  "$aws/things/" THING_NAME "/shadow/update/accepted",
  "$aws/things/" THING_NAME "/shadow/update/rejected",
  "$aws/things/" THING_NAME "/shadow/update/delta",
  "$aws/things/" THING_NAME "/shadow/get/accepted",
  "$aws/things/" THING_NAME "/shadow/get/rejected"
};

/* Amazon Root CA can be download here:
 * https://docs.aws.amazon.com/iot/latest/developerguide/server-authentication.html?icmpid=docs_iot_console#server-authentication-certs
 */
char* rootCABuff = \
// Amazon Root CA 1 RSA 2048
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

/* Fill in YOUR certificate.pem.crt with LINE ENDINGS */
char* certificateBuff = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDWjCCAkKgAwIBAgIVAPsRFvhxpgcxvXwyMKczt/eeebcAMA0GCSqGSIb3DQEB\n" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA3MDYwNjUy\n" \
"MzZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDKcYAWmFZwzGadgT1P\n" \
"4LK4uCs9vx9iHisnXl8pC1DeBXxKt0QB2InvTtXGahjmUSLqnOBthabAxNTu+mo3\n" \
"N96KpQ1mEdBldkFfJtMG+2bQaFzjOEqe0k3qFNqQdaM8JQkg+dv2VkhFoXVxrnwH\n" \
"WKcwWcqWjexmb9hVFYKm8AlaNHyh+yHsv5J0xg9KP/2ThWqVG89kx/ssei+kwNhD\n" \
"9pK7ghGp1dszMkvS1wdTHcQJj1b3UC+22soJj1CICBqmOxMVBbQT575KNKFi+UOK\n" \
"yneF3oIYgwnDOtX5kxHq3Cw0cUteaqYmzYKITIUYBxXq5B2CRZJsrNG46FK49ZNV\n" \
"3TR/AgMBAAGjYDBeMB8GA1UdIwQYMBaAFFJEaISt+QJLn05x8h7kXmC5WaQpMB0G\n" \
"A1UdDgQWBBTQuDC06walAD36MRM0Cz9TtRCPlzAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAfIzoSivJZyzay61tW0tMkqs4\n" \
"EXg3eXIHohXfkkxqKJNN4nUrp7hMYF1I/HPf/6+JL9/1tvEth6QjvW9xfXFptivJ\n" \
"KvgIMyjqzILVpi5hUxM9ewdQJFK0b+v1X/VtkXHvgXYAHTceDOVC39blz/W7mjXW\n" \
"wDKB69E8Hhp4/8YP0Bs+GOvAM8pnxJrVoNnCHpjerDFDSjmNoq33iaNtPws8FCbq\n" \
"Dd65aHrDlcyRSYp+lC2Ovg0w2v2ECWWLuZDZOPPuyRcdcRAbLiAJYLakjE9G/nBw\n" \
"NbQjhqs3h0r43SWbUOzunJbJfpbWEBMDEB2gke2adCyD+lFauwTyQDb+nXrSHA==\n" \
"-----END CERTIFICATE-----\n";

/* Fill in YOUR private.pem.key with LINE ENDINGS */
char* privateKeyBuff = \
"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEAynGAFphWcMxmnYE9T+CyuLgrPb8fYh4rJ15fKQtQ3gV8SrdE\n" \
"AdiJ707VxmoY5lEi6pzgbYWmwMTU7vpqNzfeiqUNZhHQZXZBXybTBvtm0Ghc4zhK\n" \
"ntJN6hTakHWjPCUJIPnb9lZIRaF1ca58B1inMFnKlo3sZm/YVRWCpvAJWjR8ofsh\n" \
"7L+SdMYPSj/9k4VqlRvPZMf7LHovpMDYQ/aSu4IRqdXbMzJL0tcHUx3ECY9W91Av\n" \
"ttrKCY9QiAgapjsTFQW0E+e+SjShYvlDisp3hd6CGIMJwzrV+ZMR6twsNHFLXmqm\n" \
"Js2CiEyFGAcV6uQdgkWSbKzRuOhSuPWTVd00fwIDAQABAoIBACRUZAkqA3EwrUWc\n" \
"uNUJ3OjbXB8zmj0wbswc2nTyK+v8DgcWhmTSaEaC3khCJ//FFcAGsRvWfUtgx1mv\n" \
"pLoOE6jL3/CuR7i9lROvUvWQzi2eXPUF2BZSZVsZrPN5uPaFKvM5A6lcaJHeXMYu\n" \
"6uEinrZFlt6Xz5G+hpMWTiEJOJRwPx+FDOYGtiJ4QB0LABpoSbyZDMmjwQ3qG2gy\n" \
"ypJLBlQN0J/Rzzp5F4gQR51GXb0DWiiPPKsmbrK0Qoy872q5qInkYTtf2vOu8QWD\n" \
"b5a3YZKBNpunNW1D+C1sHgRB53uh6dP6oMsD5+y80oTdTWHPObXqm6C9IrVG99Q5\n" \
"qfKHuZkCgYEA67rq7j6qpGKxC+7AVY6FY5Tn0mHnRNob23PmUqsGhFA2lh3YAdoA\n" \
"tT1t7XQy4oqlgkJQkZMzzoEfx8/FluS6L4k9o9d8anUFC4ZUWwORNSzp6AWpUmTQ\n" \
"nvzgXz6sZRXLE6SvcxnKKCz1r/JRnAWbBwrMcgivXQj1Qff5J7uXgJ0CgYEA29nY\n" \
"1vOXgX5dIsK28OLW4oWjeEMOc3YQBsf/kvXehT4gIUmjA8805YznYKqRkjZthEI2\n" \
"mIlo/mzK7XTyNp8cNh4UDH9ZtgLmCqcnLkQS1ZpN4xUj1Kgb3cGYOEqKz/0pyc4j\n" \
"fUiYdQz76CgV2rvkuzH28ul9yJFaMcqu7TLBmMsCgYEAs2gA4O3ExXNvN5uvue3M\n" \
"SyvK+20TrG0rAmxFAOm0/gJmaP2xpkcaftWP0ka7a3sF2AtMcl7L8QCJBXN1+A+A\n" \
"PCmBRi78O7Fv84N6tAIVA5l5M+MLKPAdbsMaqKJ0ZkKd2gdemIUqPEVrf0s3HT2O\n" \
"7V8kli1R+caVkm19rOWyumkCgYBXZqWj3rdOjbPtC8e6wBN5fnNcbhwinr8EUyIx\n" \
"hHlwMyCG20VpUEjRMVESxvyUSfsD52bZAVeyPgH2oNViDaMjiJWj2kUJ7OETPxx+\n" \
"ciWisXNhYZgbBLH/WOVxC4Ff6l9gkQje16t0bTMAOl9l0+i82sI4E8cjo1VeEhu0\n" \
"xMbePQKBgHHbf5YnKLn31dfZnO/O1FBqfVufGfWLgvRMxpUqcvd+gFPxlUFa3t9i\n" \
"0M4H0wTTAjO8HXBKAZv5smzOZqiuoFBOZkfAtE0+Sqail0RgjalW8DOXvN70wGwO\n" \
"gzhEPg4rD/O3kPjwy79JMvMANOnvXZ7RDAqUyve5r8FEWCiOnoLH\n" \
"-----END RSA PRIVATE KEY-----\n";

int led_pin = 10;
int led_state = 1;

void updateLedState(int desired_led_state) {
  printf("change led_state to %d\r\n", desired_led_state);
  led_state = desired_led_state;
  digitalWrite(led_pin, led_state);

  sprintf(publishPayload, "{\"state\":{\"reported\":{\"led\":%d}},\"clientToken\":\"%s\"}",
    led_state,
    clientId
  );
  client.publish(publishTopic, publishPayload);
  printf("Publish [%s] %s\r\n", publishTopic, publishPayload);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char buf[MQTT_MAX_PACKET_SIZE];
  char *pch;
  int desired_led_state;

  strncpy(buf, (const char *)payload, length);
  buf[length] = '\0';
  printf("Message arrived [%s] %s\r\n", topic, buf);

  if ((strstr(topic, "/shadow/get/accepted") != NULL) || (strstr(topic, "/shadow/update/accepted") != NULL)) {
    pch = strstr(buf, "\"reported\":{\"led\":");
    if (pch != NULL) {
      pch += strlen("\"reported\":{\"led\":");
      desired_led_state = *pch - '0';
      if (desired_led_state != led_state) {
        updateLedState(desired_led_state);
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("\r\n Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId)) {
      Serial.println("connected");

      for (int i=0; i<5; i++) {
        client.subscribe(subscribeTopic[i]);
      }

      sprintf(publishPayload, "{\"state\":{\"reported\":{\"led\":%d}},\"clientToken\":\"%s\"}",
        led_state,
        clientId
      );
      client.publish(publishTopic, publishPayload);
      printf("Publish [%s] %s\r\n", publishTopic, publishPayload);

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
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, led_state);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) break;
    // retry after 1 second
    delay(1000);
  }

  wifiClient.setRootCA((unsigned char*)rootCABuff);
  wifiClient.setClientCertificate((unsigned char*)certificateBuff, (unsigned char*)privateKeyBuff);

  client.setServer(mqttServer, 8883);
  client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
}