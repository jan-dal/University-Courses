
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "request.h"

static char buffer[BUFFER_SIZE];
static int msg_len;

void handle_connection(int consockfd) {
    fd_set descriptors;    
    FD_ZERO(&descriptors);
    struct timeval tv;
    tv.tv_sec = ONE_MINUTE;
    tv.tv_usec = 0;

    int ready = 42;
	int result = 42;
    while(ready) {
        FD_SET(consockfd, &descriptors);
        ready = select(consockfd + 1, &descriptors, NULL, NULL, &tv);
        
        if (ready == 1) {
            printf("\n\nHANDLING NEW REQUEST\n\n");
            result = handle_request(consockfd);
        } else if (ready == -1) {
            handle_error("An error occured while waiting for a socket to be ready", consockfd);
        }
		if (result > 0) {
			memset(buffer, 0, BUFFER_SIZE);
			msg_len = 0;
		} else if (result == 0) {
			printf("CLIENT DISCONECTED\n");
			break;
		}
		tv.tv_sec = ONE_MINUTE;
    }
    
	printf("CLOSING CONNECTION\n");
    if (close(consockfd) == -1) {
		perror("Could not close connection socket.");
	}
}

int handle_request(int consockfd) {
    ssize_t bytes_read = recv(consockfd, buffer+msg_len, BUFFER_SIZE-msg_len, MSG_DONTWAIT);
	if (bytes_read == -1) {
        handle_error("An error occured while receiveing request", consockfd);
    } else if (bytes_read == 0) {
		return 0;
	}
	
	char * msg_end = strstr(buffer, "\r\n");
	if (msg_end == NULL) {
		msg_len += bytes_read;
		return -1;
	}
	msg_len = msg_end - buffer;
	printf("Recieved request: %s\nlength: %d\n", buffer, msg_len);

	char * response = malloc(sizeof(char)*(msg_len+2));	
	reverse(msg_end, response, msg_len);
	ssize_t bytes_send = send(consockfd, response, msg_len+2, 0);
	
	if (bytes_send == -1) {
		handle_error("Error while sending response.", consockfd);
	}
	free(response);
    return (int)bytes_send;
}

void reverse(char * end, char * dest, int len) {
	for (int i=0; i < len; i++) {
		dest[i] = end[-i-1];	
	}
	dest[len] = '\r';
	dest[len+1] = '\n';
}

void handle_error(char * msg, int sockfd) {
	perror(msg);
	if (close(sockfd) == -1) {
		perror("Could not close socket after error.");
	}
	exit(1);
}

