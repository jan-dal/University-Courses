
#define TIMEOUT 1000 // microseconds

uint8_t validate_options(int valid_ip, uint16_t port, const char* out_file, uint32_t bytes);
int timeout_for_segment(struct timeval* timestamp, struct timeval* current_time);