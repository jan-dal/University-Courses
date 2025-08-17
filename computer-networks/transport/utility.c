
#include <stdio.h>
#include <unistd.h>
#include <error.h>
#include <inttypes.h>
#include <sys/time.h>

#include "utility.h"

uint8_t validate_options(int valid_ip, uint16_t port, const char* out_file, uint32_t bytes) {
	uint8_t no_error = 1;

	if (!port) {
		printf("Wrong port number.\n");
        no_error = 0;
	}
	if (!bytes || bytes > 10000000) {
		printf("Wrong number of bytes.\n");
        no_error = 0;
    }
    if (!valid_ip) {
        printf("Wrong server ip.\n");
        no_error = 0;
    } else if (valid_ip == -1) {
        perror("There was an error while parsing server ip address");
        no_error = 0;
    }
    if (access(out_file, F_OK) == -1) {
        perror("Output file existence");
        no_error = 0;
    } else if (access(out_file, W_OK) == -1) {
        perror("Output file write error");
        no_error = 0;
    }

    return no_error;
}

int timeout_for_segment(struct timeval* timestamp, struct timeval* current_time) {
    struct timeval time_diff;
    timersub(current_time, timestamp, &time_diff);

    return (time_diff.tv_sec * 1000000) + time_diff.tv_usec > TIMEOUT;
}