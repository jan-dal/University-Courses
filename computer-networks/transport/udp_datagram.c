
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>

#include "udp_datagram.h"

int send_udp(int sockfd, uint16_t port, struct in_addr addr, int32_t start, uint16_t size)
{
    char data[20];
    if (snprintf(data, sizeof(data), "GET %d %d\n", start, size) < 0) {
        printf("Could not compose a datagram message");
        exit(1);
    }

    struct sockaddr_in recipient;

    recipient.sin_family = AF_INET;
    recipient.sin_port = htons(port);
    recipient.sin_addr.s_addr = addr.s_addr;

    ssize_t bytes_sent = sendto(
        sockfd,
        data,
        sizeof(data),
        0,
        (struct sockaddr*)&recipient,
        sizeof(recipient)
    );

    if (bytes_sent == -1) {
        perror("An error occurred while sending udp packet");
        exit(1);
    }
    return 1;
}

int read_datagram(int sockfd, uint32_t* lar, FILE* file, struct segment* sliding_window, int32_t num_segments)
{
	static uint32_t total_rec;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    char buffer[IP_MAXPACKET + 1];

    ssize_t datagram_len = recvfrom (
        sockfd, 
        buffer, 
        IP_MAXPACKET, 
        MSG_DONTWAIT,
        (struct sockaddr*)&sender, 
        &sender_len
    );

    if (datagram_len == -1) {
        if (errno == EAGAIN) {
            return 0;
        }
        perror("An error occured while recieving a packet");
        exit(1);
    }

    if (strncmp("DATA ", buffer, 5) != 0) {
        return 0;
    }

    char * s = buffer + 5;
    char** index = &s;
    uint32_t start = strtol(&buffer[5], index, 10);
    uint32_t size  = strtol(*index, NULL, 10);

    if (size == 0) {
        return 0;
    }

    uint32_t segment = (start / SEGMENT_SIZE);

    if (*lar <= segment && segment < *lar + SWS) {
        uint32_t index = segment % SWS;

        if (!sliding_window[index].recieved) {
            total_rec += 1;
            sliding_window[index].recieved = 1;
            sliding_window[index].size = size;
            memcpy(&sliding_window[index].data, &buffer[datagram_len - size], size);

            printf("Done: %.3f%%\n", (double)total_rec / num_segments * 100);
        }

        if (*lar == segment) {
            while (sliding_window[index].recieved) {
                if (write(fileno(file), sliding_window[index].data, sliding_window[index].size) == -1) {
                    perror("An error accured while writind to file");
                    exit(1);
                }
                sliding_window[index].recieved = 0;
                *lar += 1;
                index = (index + 1) % SWS;
            }
        }
    }

    return 1;
}