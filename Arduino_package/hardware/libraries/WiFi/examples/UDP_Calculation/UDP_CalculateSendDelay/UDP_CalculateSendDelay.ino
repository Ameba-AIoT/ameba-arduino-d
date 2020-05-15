/*
   This sketch provide a simple way to roughly calculate the delay of Ameba send delay.
   The source code is separate into two parts.
   The first part is Ameba code which play sender role.
   The second part is PC code wich play receiver role. Please compile the second part and run it.
*/

#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "mynetwork";      //  your network SSID (name)
char pass[] = "mypassword";     // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;               // your network key Index number (needed only for WEP)

WiFiUDP Udp;
char server[] ="192.168.1.65";
int port = 5001;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        if (status == WL_CONNECTED) {
            break;
        }
        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("\n\rConnected to wifi");
}

char buf[256];
void loop() {
    sprintf(buf, "%d", ((int)(millis())));
    Udp.beginPacket(server, port);
    Udp.write(buf);
    Udp.endPacket();
    delay(5);
}

/***** SECOND PART: Compile below code under PC environment and run it ******/

#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define BUFSIZE 1024
#define PORT 5001

long get_current_time_with_ms (void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    long millisecondsSinceEpoch = ((long)(tv.tv_sec)) * 1000 + ((long)(tv.tv_usec)) / 1000;

    return millisecondsSinceEpoch;
}

long early_diff = 0;

long ameba_epoch = 0;
long sys_epoch = 0;

long datacount = 0;
long total_shift = 0;

void process_data(char *buf) {
    long data;
    long timediff;
    long current_epoch;

    // Packet content is Ameba's current system time
    data = atol(buf);

    if (ameba_epoch == 0) {
        /* We sync Ameba's system and PC's system time on first packet */
        ameba_epoch = data;
        sys_epoch = get_current_time_with_ms();
    } else {
        /* Calculate the delay */
        current_epoch = get_current_time_with_ms();
        timediff = (current_epoch - sys_epoch) - (data - ameba_epoch);

        if (-timediff > early_diff) {
            /* This packet came in earlier than expected. It means there is some delay at first packet. Record this deley. */
            early_diff = -timediff;
        }

        total_shift += timediff;
        datacount++;

        if (datacount % 10000 == 0) {
            printf("data count: %ld\t average delay:%f ms\r\n", datacount, (early_diff + total_shift * 1.0 / datacount));
        }
    }
}

int main(int argc, char **argv) {

    int sockfd, optval;

    struct sockaddr_in serveraddr, clientaddr;
    int clientaddr_len = sizeof(clientaddr);

    struct hostent *hostp;
    char *hostaddrp;

    int n;
    char buf[BUFSIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\r\n");
        return -1;
    }

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        printf("ERROR on binding\r\n");
    }

    while (1) {
        memset(buf, 0, BUFSIZE);
        n = recvfrom(sockfd, buf, BUFSIZE-1, 0, (struct sockaddr *) &clientaddr, &clientaddr_len);
        if (n < 0) {
            printf("ERROR in recvfrom\r\n");
            return -1;
        }
        process_data(buf);
    }
    return 0;
}

#endif
