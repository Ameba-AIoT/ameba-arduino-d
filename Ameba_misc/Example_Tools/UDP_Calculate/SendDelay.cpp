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

        if (datacount % 1000 == 0) {
            printf("\r\ndata count: %ld\t average delay:%f ms", datacount, (early_diff + total_shift * 1.0 / datacount));
        }
    }
}

int main(int argc, char **argv) {
    printf("Please wait.");

    int sockfd, optval;

    struct sockaddr_in serveraddr, clientaddr;
#ifdef __CYGWIN__
    int clientaddr_len = sizeof(clientaddr);
#else
    unsigned int clientaddr_len = sizeof(clientaddr);
#endif

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
        n = recvfrom(sockfd, buf, (BUFSIZE - 1), 0, (struct sockaddr *)(&clientaddr), &clientaddr_len);
        if (n < 0) {
            printf("\r\nERROR in recvfrom\r\n");
            return -1;
        }
        process_data(buf);
    }
    return 0;
}
