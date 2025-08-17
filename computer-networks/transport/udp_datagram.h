
#define SWS 2500 // segments
#define SEGMENT_SIZE 1000 // bytes

struct segment {
    uint8_t recieved;
    uint16_t size;
    struct timeval timestamp;
    char data[SEGMENT_SIZE];
};

int send_udp(int sockfd, uint16_t port, struct in_addr addr, int32_t start, uint16_t size);
int read_datagram(int sockfd, uint32_t* lar, FILE* file, struct segment* sliding_window, int32_t num_segments);