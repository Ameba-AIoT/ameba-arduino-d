/*
   This sketch provide a way to measure proper udp timeout value via dynamically changing udp
   receiving timeout value. If timeout happens, then add 1 to receiving timeout. Otherwise
   decrease receiving timeout.
   This sketch separate into two parts:
   The first part is Arduino code which play receiver role.
   The second part is PC code wich play sender role. Please compile the second part and run it.

   You can open Serial Plotter to check the change behavor of timeout value.
   The meaning of timeout value depends on the sending frequency from sender side.
   If the sender side send packets frequently, then the Arduino side can have smaller receiving timeout value
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

int timeout = 1000;
char buf[256];
void loop() {
    int n;
    while (1) {
        memset(buf, 0, 256);
        n = Udp.read(buf, 255);

        /* You can change different penalty way here. (Ex. two continuous timeouts add 10 to timeout value) */
        if (n <= 0) {
            timeout++;
            if (timeout > 3000) {
                // assume that the udp timeout is no more than 3s
                timeout = 3000;
            }
        } else {
            timeout--;
            if (timeout < 1) {
                timeout = 1;
            }
        }
        Serial.println(timeout);
        Udp.setRecvTimeout(timeout);
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
#include <unistd.h>

#define BUFSIZE 16

char *hostname = "192.168.1.213";
int portno = 5001;

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in serveraddr;
    int serverlen = sizeof(serveraddr);
    char buf[BUFSIZE];
    int counter = 0;

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

    while(1) {
        memset(buf, 0, BUFSIZE);
        counter = (counter + 1) % 10;
        sprintf(buf, "%d", counter);

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
