
#define PACKETS_PER_TTL 3

extern char * dest_ip;
extern u_int16_t pid_id;
extern u_int16_t sequence;
extern uint8_t end;
extern struct timeval rtt[];

void send_icmp_echo_request(int sockfd, int ttl, int n);
int read_awaiting_packet(int sockfd);
u_int16_t compute_icmp_checksum (const void *buff, int length);
int record_rtt(uint16_t res_seq, uint16_t res_id);
int check_response(u_int8_t *raw_response);
void print_sender_ip(char * sender_ip);