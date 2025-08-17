
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "request.h"

int main(int argc, char** argv)
{

	if (argc != 3) {
		printf("Usage: %s port base_directory\n", argv[0]);
		exit(1);
	}

	uint16_t port = atoi(argv[1]);

	struct path base_dir;
	base_dir.path = argv[2];
	base_dir.length = strlen(base_dir.path);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("An error occured while trying to open a socket");
		exit(1);
	}

	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
		perror("An error occured while binding socket to port");
		exit(1);
	}
	if (listen(sockfd, 1024) == -1) {
		perror("An error occured while listening on socked");
		exit(1);
	}

	while(1) {
		fd_set descriptors;
		FD_ZERO(&descriptors);
		FD_SET(sockfd, &descriptors);
		
		int ready = select(sockfd + 1, &descriptors, NULL, NULL, NULL);
		if (ready == 1) {
			printf("\n\nOPENING NEW CONNECTION\n\n");
			establish_connection(sockfd, &base_dir);
		} else if (ready == -1) {
			perror("An error occured while waiting for a socket to be ready");
			exit(1);
		}
	}

	return 0;
}