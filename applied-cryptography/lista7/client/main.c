
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <openssl/ssl.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "request.h"
#include "ossl.h"

int main(int argc, char **argv)
{
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror("Could not explicitly ignore SIGCHLD.");
		exit(1);
	}

	SSL_CTX *ctx;
	ctx = create_context();
	configure_context(ctx);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		handle_error("An error occured while trying to open a socket.", sockfd);
	}

	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(7777);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
		handle_error("An error occured while listening on socket.", sockfd);
	}

	fd_set descriptors;
	FD_ZERO(&descriptors);
	while(1) {
		FD_SET(sockfd, &descriptors);
		FD_SET(STDIN_FILENO, &descriptors);

		//char *line = readline("Enter a message: ");
		//ssize_t send_bytes = send(sockfd, line, sizeof(*line), 0);

		int ready = select(sockfd + 1, &descriptors, NULL, NULL, NULL);
		
		if (ready >= 1) {
			if (FD_ISSET(sockfd, &descriptors)) {
				char buffer[BUFFER_SIZE] = {0};
				ssize_t bytes_read = recv(sockfd, buffer, BUFFER_SIZE, 0);
				
				printf("%s\n", buffer);
			}
			if (FD_ISSET(STDIN_FILENO, &descriptors)) {	
				size_t s = 0;
				size_t *n = &s;
				char *line = NULL;
				char **lptr = &line; 
				ssize_t line_length = getline(lptr, n, stdin);

				line[line_length-1] = '\r';
				line[line_length] = '\n';

				send(sockfd, line, line_length+1, 0);
				if (line != NULL) {
					free(line);
				}
			}
		} else if (ready == -1) {
			handle_error("An error occured while waiting for a socket to be ready.", sockfd);
		}
	}
	if (close(sockfd) == -1) {
		perror("Could not close the listening socket.");
	}
	SSL_CTX_free(ctx);
	return 0;
}
