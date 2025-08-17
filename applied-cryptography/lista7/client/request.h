
#define BUFFER_SIZE 65535
#define ONE_MINUTE 60

void handle_connection(int sockfd, SSL_CTX *ctx);
int handle_request(SSL *ssl);
void handle_error(char * msg, int socket);
void reverse(char * end, char * dest, int len);
