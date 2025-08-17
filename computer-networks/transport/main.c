
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "utility.h"
#include "udp_datagram.h"

static char* opt_error_msg = "Error parsing command line arguments.";

int main(int argc, char** argv)
{
	if (argc != 5) {
		printf("%s\nUsage: %s ip port output_file bytes\n", opt_error_msg, argv[0]);
		exit(1);
	}

	struct in_addr server_ip;

	int valid_ip = inet_pton(AF_INET, argv[1], &server_ip);
	uint16_t port = atoi(argv[2]);
	char* output_file = argv[3];
	uint32_t num_bytes = atoi(argv[4]);

	if(!validate_options(valid_ip, port, output_file, num_bytes)) {
		printf("\n%s\nUsage: %s ip port output_file bytes\n", opt_error_msg, argv[0]);
		exit(1);
	}

    static struct segment sliding_window[SWS]; 

	FILE* file = fopen(output_file, "w");

	uint32_t lar = 0;
	uint32_t num_segments = (num_bytes / SEGMENT_SIZE) + ((num_bytes % SEGMENT_SIZE) != 0) ;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("An error occured while trying to open a socket");
		exit(1);
	}

	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(12345);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		perror("An error occured while binding socket to port");
		exit(1);
	}

	while (lar < num_segments) {
		for (uint32_t i = 0; i < SWS && (i + lar) < num_segments; i++){
			uint32_t index = (lar + i) % SWS;
			struct timeval current_time;
        	if(gettimeofday(&current_time, NULL) == -1){
            	perror("An error occured while getting time of day");
            	exit(1);
        	};

			if (!sliding_window[index].recieved && timeout_for_segment(&sliding_window[index].timestamp, &current_time)) {
				uint32_t start = (i + lar) * SEGMENT_SIZE;
				uint16_t size =  num_bytes - start < SEGMENT_SIZE ? num_bytes - start : SEGMENT_SIZE; 
				sliding_window[index].timestamp.tv_sec = current_time.tv_sec;
				sliding_window[index].timestamp.tv_usec = current_time.tv_usec;

				send_udp(sockfd, port, server_ip, start, size);
			}
			read_datagram(sockfd, &lar, file, sliding_window, num_segments);
		}

	}

	return 1;
}