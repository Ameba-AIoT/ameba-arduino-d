/*
   This sketch provide a simple way to roughly calculate the delay of Ameba receive delay.
   The source code is separate into two parts.
   The first part is Ameba code which play receiver role.
   The second part is PC code wich play sender role. Please compile the second part and run it.
*/

#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdio.h>

int status = WL_IDLE_STATUS;
char ssid[] = "mynetwork";      //  your network SSID (name)
char pass[] = "mypassword";     // your network password
int keyIndex = 0;               // your network key Index number (needed only for WEP)

unsigned int localPort = 5001;  // local port to listen for UDP packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

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
    Udp.begin(localPort);
}

char buf[256];
void loop() {
    int n;
    while (1) {
        memset(buf, 0, 256);
        n = Udp.read(buf, 255);
        if (n > 0) {
            handle_data(buf);
        }
    }
}

long early_diff = 0;

long ameba_epoch = 0;
long long sys_epoch = 0;

long datacount = 0;
long total_shift = 0;

void handle_data(char *buf) {
    long data;
    long timediff;
    long current_epoch;

    data = atol(buf);
    if (ameba_epoch == 0) {
        /* We sync Ameba's system and PC's system time on first packet */
        ameba_epoch = millis();
        sys_epoch = data;
    } else {
        current_epoch = millis();
        timediff = (current_epoch - ameba_epoch) - (data - sys_epoch);

        if (-timediff > early_diff) {
            /* This packet came in earlier than expected. It means there is some delay at first packet. Record this deley. */
            early_diff = -timediff;
        }

        total_shift += timediff;
        datacount++;

        if (datacount % 10000 == 0) {
            Serial.print("data count: ");
            Serial.print(datacount);
            Serial.print("\taverage delay: ");
            Serial.print(early_diff + total_shift * 1.0 / datacount);
            Serial.println(" ms");
        }
    }
}

/***** SECOND PART: Compile below code under PC environment and run it ******/

#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFSIZE 1024

char *hostname = "192.168.1.212";
int portno = 5001;

long base_current_time = 0;
long get_current_time_with_ms (void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    long millisecondsSinceEpoch = ((long)(tv.tv_sec)) * 1000 + ((long)(tv.tv_usec)) / 1000;

    if (base_current_time == 0) {
        base_current_time = millisecondsSinceEpoch;
        return 0;
    } else {
        return millisecondsSinceEpoch - base_current_time;
    }
}

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in serveraddr;
    int serverlen = sizeof(serveraddr);
    char buf[BUFSIZE];

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\r\n");
        return -1;
    }

    /* build the server's Internet address */
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(hostname);
    serveraddr.sin_port = htons(portno);

    while (1) {
        memset(buf, 0, BUFSIZE);
        sprintf(buf, "%ld", get_current_time_with_ms());

        /* send the message to the server */
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&serveraddr, serverlen);
        if (n < 0) {
            printf("ERROR in sendto\r\n");
            return -1;
        }
        usleep(5 * 1000);
    }

    return 0;
}

#endif
