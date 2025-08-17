
#define SIMPLE_RESPONSE_FORMAT "%s\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s"
#define RESPONSE_FORMAT_200    "%s\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n"
#define RESPONSE_FORMAT_301    "%s\r\nContent-Type: %s\r\nLocation: %s\r\nContent-Length: %ld\r\n\r\n%s"

#define OK                "HTTP/1.1 200 OK"
#define FORBIDDEN         "HTTP/1.1 403 Forbidden"
#define NOT_FOUND         "HTTP/1.1 404 Not Found"
#define NOT_IMPLEMENTED   "HTTP/1.1 501 Not Implemented"
#define MOVED_PERMANENTLY "HTTP/1.1 301 Moved Permanently"

#define RESPONSE_403 "Access forbidden."
#define RESPONSE_404 "The resource could not be found." 
#define RESPONSE_501 "This functionality was not implemented."
#define RESPONSE_301 "The resource was moved to a different location."

#define RESPONSE_301_SIZE sizeof(RESPONSE_301)-1
#define RESPONSE_403_SIZE sizeof(RESPONSE_403)-1
#define RESPONSE_404_SIZE sizeof(RESPONSE_404)-1
#define RESPONSE_501_SIZE sizeof(RESPONSE_501)-1

#define PNG_TYPE     "image/png"
#define JPEG_TYPE    "image/jpeg"
#define PDF_TYPE     "application/pdf"
#define CSS_TYPE     "text/css;charset=utf-8"
#define HTML_TYPE    "text/html;charset=utf-8"
#define TXT_TYPE     "text/plain;charset=utf-8"
#define DEFAULT_TYPE "application/octet-stream"

#define INITIAL_SIZE 1024

int respond(int connected_sockfd, struct http_request* request, struct path* base_dir);
int read_file(const char* file_path, char** response_ptr, int size);
int get_filesize(const char* file_path);

char* get_content_type(struct path* path);
char* create_expanded_file_path(struct path* base_dir, struct path* host, struct path* path);
char* create_redirect_path(struct path* path);

int is_directory(char* path);
int check_permissions(struct path* base_dir, struct path* host, char* file_path);