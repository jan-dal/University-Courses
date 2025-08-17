

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "configuration.h"
#include "udp_packet.h"

int send_udp_packet(struct in_addr address, struct vec_element* vec)
{
    char data[9];
    struct sockaddr_in recipient;

    recipient.sin_family = AF_INET;
    recipient.sin_port   = htons(54321);
    recipient.sin_addr.s_addr   = address.s_addr;
    
    data[0] = (char)(vec->address.s_addr >> 24);
    data[1] = (char)(vec->address.s_addr >> 16);
    data[2] = (char)(vec->address.s_addr >> 8);
    data[3] = (char)vec->address.s_addr;
    data[4] = (char)vec->mask;
    
    data[5] = (char)(vec->distance >> 24);
    data[6] = (char)(vec->distance >> 16);
    data[7] = (char)(vec->distance >> 8);
    data[8] = (char)vec->distance;

    ssize_t bytes_sent = sendto(
        sockfd,
        data,
        sizeof(data),
        0,
        (struct sockaddr*)&recipient,
        sizeof(recipient)
    );

    if (bytes_sent == -1) {
        if (errno == ENETUNREACH) {
            return 0;
        } else {
            perror("An error occurred while sending udp packet");
            exit(1);
        }
    }

    return 1;
}

int send_broadcast(struct conf_element * conf, struct vec_element* vec)
{
    struct in_addr br_addr;
    br_addr.s_addr = broadcast_address(conf->address.s_addr, conf->mask);
    return send_udp_packet(br_addr, vec);
}

int recieve_udp()
{
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    uint8_t buffer[IP_MAXPACKET];

    ssize_t datagram_len = recvfrom (
        sockfd, 
        buffer, 
        IP_MAXPACKET, 
        MSG_DONTWAIT,
        (struct sockaddr*)&sender, 
        &sender_len
    );

    if (datagram_len == -1) {
        perror("An error occured while recieving a packet");
        exit(1);
    }

    if (datagram_len != 9) {
        return 0;
    }

    int index = find_config_index(&sender.sin_addr);
    if (index == -1) {
        return 0;
    } else {
        conf.conf_table[index].up = 1;
		conf.conf_table[index].not_responded = 0;
    }

    struct vec_element rec_vec;

    rec_vec.address.s_addr = ((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3]);
    rec_vec.mask = buffer[4];
    rec_vec.distance = (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
    rec_vec.via = &conf.conf_table[index];
    rec_vec.direct = network_address(rec_vec.address.s_addr, rec_vec.mask) == network_address(rec_vec.via->address.s_addr, rec_vec.via->mask);

    if (inet_ntop(
        AF_INET,
        &rec_vec.address,
        rec_vec.address_repr,
        sizeof(rec_vec.address_repr)
    ) == NULL) {
        perror("An error occured while converting address to string");
        exit(1);
    }
    
    // printf("Recieved vector\nAddress: %s\nMask: %d\nDistance: %u\n", rec_vec.address_repr, rec_vec.mask, rec_vec.distance);

    update_routing_table(&rec_vec);
    return 1;
}