
#define UDP_PROTOCOL 17

int recieve_udp();
int send_broadcast(struct conf_element * conf, struct vec_element* vec);
int send_udp_packet(struct in_addr address, struct vec_element* vec);