
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "request.h"
#include "response.h"

int establish_connection(int sockfd, struct path* base_dir) {
    int connected_sockfd = accept(sockfd, NULL, NULL);
    if (connected_sockfd == -1) {
        perror("An error occured while accepting connection");
        exit(1);
    }
    fd_set descriptors;    
    struct timeval tv;
    tv.tv_sec = 4;
    tv.tv_usec = 0;

    int ready = 42;
    while(ready) {
        FD_ZERO(&descriptors);
        FD_SET(connected_sockfd, &descriptors);
        ready = select(connected_sockfd + 1, &descriptors, NULL, NULL, &tv);
        
        if (ready == 1) {
            printf("\n\nREADING NEW REQUEST\n\n");
            struct http_request* request = read_request(connected_sockfd);
            if (request->client_disconnected) {
                printf("CLIENT DISCONECTED\n");
                ready = 0;
            } else if (request->method == METHOD_GET) {
                printf("\n\nRESPONDING\n\n");
                respond(connected_sockfd, request, base_dir);
                printf("\n\nRESPONSE SENT\n\n");  
                if (request->connection == CONNECTION_CLOSE) {
                    printf("CONNECTION CLOSE HEADER\n");
                    ready = 0;
                } else if (request->connection == CONNECTION_KEEP_ALIVE) {
                    tv.tv_sec = 1; 
                }  
            }
            clear_request(request);
        } else if (ready == -1) {
            perror("An error occured while waiting for a socket to be ready");
            exit(1);
        }
    }
    printf("CLOSING CONNECTION\n");
    close(connected_sockfd);
    return 1;
}

struct http_request* read_request(int connected_sockfd) {
    char buffer[BUFFER_SIZE+1] = {0};
    ssize_t bytes_read = recv(connected_sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
    if (bytes_read == -1) {
        perror("An error occured while receiveing request");
        exit(1);
    }
    return parse_request(buffer, bytes_read);
}

struct http_request* parse_request(char *buffer, int bytes_read) {
    struct http_request* request = malloc(sizeof(struct http_request));
    request->path.path = NULL;
    request->host.path = NULL;
    request->client_disconnected = 0;

    if (bytes_read == 0) {
        request->client_disconnected = 1;
        return request;
    }
    
    char* line = buffer;
    do {
        char key[1000];
        char value[1000];

        if (sscanf(line, "%s %s", key, value) == EOF) {
            break;
        }
        printf("------- %s %s\n", key, value);
        
        if (strstr(key, "GET") != NULL) {
            request->method = METHOD_GET;
            
            int path_len = strlen(value) + 1;
            request->path.length = path_len-1;
            request->path.path = malloc(sizeof(char) * path_len);
            memcpy(request->path.path, value, path_len);
        }

        else if (strstr(key, "Host") != NULL) {
            char host[1000];
            if (sscanf(value, "%[^:]:%d", host, &request->port) == EOF) {
                perror("Couldn't read host");
                exit(1);
            }
            int host_len = strlen(host) + 1;
            request->host.length = host_len-1;
            request->host.path = malloc(sizeof(char) * host_len);
            memcpy(request->host.path, host, host_len);
        }

        else if (strstr(key, "Connection") != NULL) {
            
            if (strncmp("close", value, 11) == 0) {
                request->connection = CONNECTION_CLOSE;
            } else {
                request->connection = CONNECTION_KEEP_ALIVE;
            }
        }

    } while ((line = (strchr(line, '\n') + 1)) != NULL);

    printf("Method: %d, path: %s, host: %s, connection: %d port: %d\n", request->method, request->path.path, request->host.path, request->connection, request->port);

    return request;
}

void clear_request(struct http_request* request) {
    if (request->path.path != NULL) {
        free(request->path.path);   
    }
    if (request->host.path != NULL) {
        free(request->host.path);
    }
    free(request);
}