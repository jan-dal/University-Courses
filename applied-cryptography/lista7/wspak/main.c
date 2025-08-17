
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#include "request.h"

int main(void)
{
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("Could not explicitly ignore SIGCHLD.");
		exit(1);
	}
	uint16_t port = 7777;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		handle_error("An error occured while trying to open a socket.", sockfd);
	}

	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
		handle_error("An error occured while binding socket to port.", sockfd);
	}
	if (listen(sockfd, 256) == -1) {
		handle_error("An error occured while listening on socket.", sockfd);
	}

	fd_set descriptors;
	FD_ZERO(&descriptors);
	while(1) {
		FD_SET(sockfd, &descriptors);
		int ready = select(sockfd + 1, &descriptors, NULL, NULL, NULL);

		if (ready == 1) {
			printf("\n\nOPENING NEW CONNECTION\n\n");
			int consockfd = accept(sockfd, NULL, NULL);
			if (consockfd == -1) {
				handle_error("An error occured while accepting connection", consockfd);
			}
			pid_t pid = fork();
			if (pid == 0) {
				handle_connection(consockfd);
				exit(0);
			}

		} else if (ready == -1) {
			handle_error("An error occured while waiting for a socket to be ready.", sockfd);
		}
	}
	if (close(sockfd) == -1) {
		perror("Could not close the listening socket.");
	}
	return 0;
}
