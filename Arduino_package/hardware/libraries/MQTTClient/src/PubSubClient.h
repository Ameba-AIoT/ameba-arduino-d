/*
 PubSubClient.h - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net
*/

#ifndef PubSubClient_h
#define PubSubClient_h

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include "Stream.h"

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4

// MQTT_VERSION : Pick the version
//#define MQTT_VERSION MQTT_VERSION_3_1
#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif

#ifdef ARDUINO_AMEBA
// PCNs (Patch Change Numbers)

/* For projects (Ex. LASS) that needs larger packets size then default. */
#define MQTT_PCN001_ENLARGE_PACKET_SIZE

/* MQTT use busy loop to check if data available. It cause CPU resource cannot be released. */
#define MQTT_PCN002_NON_BUSY_LOOP_READ

/* MQTT use busy loop to check if connected to server. It cause CPU resource cannot be released. */
#define MQTT_PCN003_NON_BUSY_LOOP_CONNECT

/* MQTT try to read even server disconnected. */
#define MQTT_PCN004_BREAK_READ_IF_NO_CONNECTION

/* If there is no imcoming data, then the loop function would become a busy loop*/
#define MQTT_PCN005_NON_BUSY_LOOP_If_NO_DATA

/* By default it won't wait for ack. It cause un-expected behavior if subscribe and publish have sequence dependency */
#define MQTT_PCN006_SUPPORT_WAIT_FOR_ACK

/* By default publish sent with qos 0. Support qos level for publish. */
#define MQTT_PCN007_SUPPORT_PUB_QOS

#endif

#ifdef MQTT_PCN001_ENLARGE_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 512
#endif

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 512
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 60
#endif

// MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT 60
#endif

// MQTT_MAX_TRANSFER_SIZE : limit how much data is passed to the network client
//  in each write call. Needed for the Arduino Wifi Shield. Leave undefined to
//  pass the entire MQTT packet in each write call.
//#define MQTT_MAX_TRANSFER_SIZE 80

// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT         -4
#define MQTT_CONNECTION_LOST            -3
#define MQTT_CONNECT_FAILED             -2
#define MQTT_DISCONNECTED               -1
#define MQTT_CONNECTED                   0
#define MQTT_CONNECT_BAD_PROTOCOL        1
#define MQTT_CONNECT_BAD_CLIENT_ID       2
#define MQTT_CONNECT_UNAVAILABLE         3
#define MQTT_CONNECT_BAD_CREDENTIALS     4
#define MQTT_CONNECT_UNAUTHORIZED        5

#define MQTTCONNECT     1U << 4  // Client request to connect to Server
#define MQTTCONNACK     2U << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3U << 4  // Publish message
#define MQTTPUBACK      4U << 4  // Publish Acknowledgment
#define MQTTPUBREC      5U << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6U << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7U << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8U << 4  // Client Subscribe request
#define MQTTSUBACK      9U << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10U << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11U << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12U << 4 // PING Request
#define MQTTPINGRESP    13U << 4 // PING Response
#define MQTTDISCONNECT  14U << 4 // Client is Disconnecting
#define MQTTReserved    15U << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

#ifdef ESP8266
#include <functional>
#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)
#endif

class PubSubClient {
    private:
        Client* _client;
        uint8_t buffer[MQTT_MAX_PACKET_SIZE];
        uint16_t nextMsgId;
        unsigned long lastOutActivity;
        unsigned long lastInActivity;
        bool pingOutstanding;
        MQTT_CALLBACK_SIGNATURE;
        uint16_t readPacket(uint8_t*);
        boolean readByte(uint8_t * result);
        boolean readByte(uint8_t * result, uint16_t * index);
        boolean write(uint8_t header, uint8_t* buf, uint16_t length);
        uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos);
        IPAddress ip;
        const char* domain;
        uint16_t port;
        Stream* stream;
        int _state;

#ifdef MQTT_PCN006_SUPPORT_WAIT_FOR_ACK
        uint8_t waitAck;
        uint8_t ackState;
#endif

#ifdef MQTT_PCN007_SUPPORT_PUB_QOS
        uint8_t pub_qos;
#endif

    public:
        PubSubClient();
        PubSubClient(Client& client);
        PubSubClient(IPAddress, uint16_t, Client& client);
        PubSubClient(IPAddress, uint16_t, Client& client, Stream&);
        PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
        PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
        PubSubClient(uint8_t *, uint16_t, Client& client);
        PubSubClient(uint8_t *, uint16_t, Client& client, Stream&);
        PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
        PubSubClient(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);
        PubSubClient(const char*, uint16_t, Client& client);
        PubSubClient(const char*, uint16_t, Client& client, Stream&);
        PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client);
        PubSubClient(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE, Client& client, Stream&);

        PubSubClient& setServer(IPAddress ip, uint16_t port);
        PubSubClient& setServer(uint8_t * ip, uint16_t port);
        PubSubClient& setServer(const char * domain, uint16_t port);
        PubSubClient& setCallback(MQTT_CALLBACK_SIGNATURE);
        PubSubClient& setClient(Client& client);
        PubSubClient& setStream(Stream& stream);

        boolean connect(const char* id);
        boolean connect(const char* id, const char* user, const char* pass);
        boolean connect(const char* id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
        boolean connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
        void disconnect();
        boolean publish(const char* topic, const char* payload);
        boolean publish(const char* topic, const char* payload, boolean retained);
        boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);
        boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
        boolean publish_P(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
        boolean subscribe(const char* topic);
        boolean subscribe(const char* topic, uint8_t qos);
        boolean unsubscribe(const char* topic);
        boolean loop();
        boolean connected();
        int state();

#ifdef MQTT_PCN006_SUPPORT_WAIT_FOR_ACK
        void waitForAck(uint8_t enable);
#endif

#ifdef MQTT_PCN007_SUPPORT_PUB_QOS
        uint8_t setPublishQos(uint8_t qos_level);
#endif
};

#endif
