
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "configuration.h"
#include "udp_datagram.h"

int main()
{
	struct routing_table routing_tab;
	struct routing_tree tree;

	read_configuration(&routing_tab, &tree);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("An error occured while trying to open a socket");
		exit(1);
	}

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(54321);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		perror("An error occured while binding socket to port");
		exit(1);
	}

	int broadcastPermission = 1;
	if (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission, sizeof(broadcastPermission)) == -1) {
		perror("An error occured while setting socket broadcast option");
		exit(1);
	}

	while(1) {
		printf("\n");
		print_routing(&routing_tab);
		
		//Send routing vectors to neighbours
		for (uint8_t i=0; i < tree.n; i++) {
			for (uint8_t j=0; j < routing_tab.n; j++) {
				if (!routing_tab.nodes[j]->removed) {
					send_udp_datagram(sockfd, routing_tab.nodes[j], &tree.neighbours[i]);
				}
			}
		}

		// Assume noone has responded in this tour
		for (uint8_t i=0; i < routing_tab.n; i++) {
			routing_tab.nodes[i]->not_responded++;
		}

		fd_set descriptors;
		struct timeval tv;
		tv.tv_sec = LISTENING_TIME;
		tv.tv_usec = 0;

		// Listen for incoming udp datagrams
		int ready = 42;
		while (ready) {
			FD_ZERO(&descriptors);
			FD_SET(sockfd, &descriptors);
			ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);

			if (ready == 1) {
				recieve_udp(sockfd, &routing_tab, &tree);
			} else if (ready == -1) {
				perror("An error occured while waiting for a socket to be ready");
				exit(1);
			}
		}

		// Check if there is timeout for some neighbour, remove some vectors
		update_distances(&routing_tab, &tree);
	}
	return 0;
}
