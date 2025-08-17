
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <arpa/inet.h>

#include "packets.h"

u_int16_t sequence = 0;
u_int16_t pid_id = 0;
char* dest_ip = "";
uint8_t end = 0;
struct timeval rtt[PACKETS_PER_TTL];

int main(int argc, char** argv)
{
	pid_id = getpid();
	if (argc > 1) {
		dest_ip = argv[1];
	}

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sockfd == -1) {
		perror("An error occured while trying to open a socket");
		exit(1);
	}

	for (int ttl = 1; ttl < 31; ttl++) {

		for (int i = 0; i < PACKETS_PER_TTL; i++) {
			send_icmp_echo_request(sockfd, ttl, i);
		}

		fd_set descriptors;
		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int n_recieved = 0;
		uint32_t avg_time = 0;

		printf("%d.\t", ttl);

		while ((tv.tv_sec > 0 || tv.tv_usec > 0) && (n_recieved < 3)) {
			FD_ZERO(&descriptors);
			FD_SET(sockfd, &descriptors);
			int ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);

			if (ready == 1) {
				if (read_awaiting_packet(sockfd)) {
					n_recieved += 1;
				};
			} else if (ready == -1) {
				perror("An error occured while waiting for a socket to be ready");
				exit(1);
			}
		}

		if (n_recieved == 0) {
			printf("*\n");
		} else if (n_recieved < PACKETS_PER_TTL) {
			printf(" ???\n");
		} else {

			for (int i = 0; i < PACKETS_PER_TTL; i++) {
				avg_time += (1000 * rtt[i].tv_sec) + (rtt[i].tv_usec / 1000);
			}

			printf("\t%dms\n", avg_time / PACKETS_PER_TTL);
		}

		sequence += PACKETS_PER_TTL;

		if (end) {
			return 0;
		}
	}
	return 0;
}
