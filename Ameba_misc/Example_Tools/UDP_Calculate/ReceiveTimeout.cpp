#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 16

//const char *hostname = "192.168.2.29";
const char *hostname = "0.0.0.0";
int portno = 5001;

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in serveraddr;
    int serverlen = sizeof(serveraddr);
    char buf[BUFSIZE];
    int counter = 0;

    hostname = argv[1];

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
