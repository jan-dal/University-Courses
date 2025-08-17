
#define BUFFER_SIZE 10000

#define CONNECTION_CLOSE 100
#define CONNECTION_KEEP_ALIVE 200

#define METHOD_GET 3

struct path {
    int length;
    char* path;
};

struct http_request {
    int method;
    struct path path;
    struct path host;
    int port;
    int connection;
    int client_disconnected;
};

int establish_connection(int sockfd, struct path* base_dir);
struct http_request* read_request(int connected_sockfd);
struct http_request* parse_request(char *buffer, int bytes_read);
void clear_request(struct http_request* request);