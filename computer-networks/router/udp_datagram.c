
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "configuration.h"
#include "udp_datagram.h"

int send_udp_datagram(int sockfd, struct node* node, struct neighbour_node* rec_node)
{   
    if (node->distance < INF) {
        node->send_inf = 0;
    }
    
    if(node->send_inf >= BROADCAST_INF && rec_node->up) {
        return 0;
    }

    if (node->distance >= INF) {
        node->send_inf++;
    }

    char data[9];
    struct sockaddr_in recipient;

    recipient.sin_family = AF_INET;
    recipient.sin_port = htons(54321);
    recipient.sin_addr.s_addr = broadcast_address(rec_node->interface_addr.s_addr, rec_node->mask);

    data[0] = (char)(node->net_addr.s_addr >> 24);
    data[1] = (char)(node->net_addr.s_addr >> 16);
    data[2] = (char)(node->net_addr.s_addr >> 8);
    data[3] = (char)node->net_addr.s_addr;
    data[4] = (char)node->mask;
    
    data[5] = (char)(node->distance >> 24);
    data[6] = (char)(node->distance >> 16);
    data[7] = (char)(node->distance >> 8);
    data[8] = (char)node->distance;

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
            rec_node->up = 0;
            for (uint8_t i=0; i < rec_node->n; i++) {
                rec_node->nodes[i].distance = INF;
            }
            return 0;
        } else {
            perror("An error occurred while sending udp packet");
            exit(1);
        }
    } else {
        rec_node->up = 1;
    }

    return 1;
}

int recieve_udp(int sockfd, struct routing_table* routing_tab, struct routing_tree* tree)
{
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    uint8_t buffer[IP_MAXPACKET + 1];

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

    // Find if sender is a neighbour
    struct neighbour_node* sender_node = find_neighbour_interface(tree, sender.sin_addr); 
    if (sender_node == NULL) {
        return 0;
    }
	sender_node->up = 1;

    struct node rec_node;

    rec_node.net_addr.s_addr = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    rec_node.distance = (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
    rec_node.mask = buffer[4];
    rec_node.via = sender_node;

    char addr[20];

    if(inet_ntop(AF_INET, &rec_node.net_addr, addr, sizeof(addr)) == NULL) {
        perror("An error occured while converting address to string");
        exit(1);
    }

    update_routing(routing_tab, &rec_node, sender_node);
    return 1;
}