
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "request.h"
#include "response.h"

int respond(int connected_sockfd, struct http_request* request, struct path* base_dir)
{
    char* file_path = create_expanded_file_path(base_dir, &request->host, &request->path);
    int is_dir = is_directory(file_path);
    // -2 -> outside of base_dir
    // -1 -> no read permission/does not exist/directory
    //  0 -> can be read
    int access_type = check_permissions(base_dir, &request->host, file_path);

    char* response = malloc(sizeof(char) * INITIAL_SIZE);
    char** response_ptr = &response;
    int response_len = 0;

    printf("is_directory: %d\n", is_dir);
    printf("access_type: %d\n", access_type);

    // 403 Forbidden
    if (access_type == -2) {
        printf("SENDING 403\n");
        snprintf(response, INITIAL_SIZE, SIMPLE_RESPONSE_FORMAT, FORBIDDEN, TXT_TYPE, RESPONSE_403_SIZE, RESPONSE_403);
        response_len = strlen(response);
    }
    // 301 Moved permanently
    else if(is_dir) {
        printf("SENDING 301\n");
        char* redirect_path = create_redirect_path(&request->path);
        snprintf(response, INITIAL_SIZE, RESPONSE_FORMAT_301, MOVED_PERMANENTLY, TXT_TYPE, redirect_path, RESPONSE_301_SIZE, RESPONSE_301);
        response_len = strlen(response);
        free(redirect_path);
    }
    // 404 Not found
    else if(access_type == -1) {
        printf("SENDING 404\n");
        snprintf(response, INITIAL_SIZE, SIMPLE_RESPONSE_FORMAT, NOT_FOUND, TXT_TYPE, RESPONSE_404_SIZE, RESPONSE_404);
        response_len = strlen(response);
    } 
    // 200 Found
    else if(access_type == 0) {
        printf("SENDING 200\n");
        char* content_type = get_content_type(&request->path);
        int filesize = get_filesize(file_path);
        snprintf(response, INITIAL_SIZE, RESPONSE_FORMAT_200, OK, content_type, filesize);
        response_len = read_file(file_path, response_ptr, filesize);
    } 
    // 501 Not implemented
    else {
        printf("SENDING 501\n");
        snprintf(response, INITIAL_SIZE, SIMPLE_RESPONSE_FORMAT, NOT_IMPLEMENTED, TXT_TYPE, RESPONSE_501_SIZE, RESPONSE_501);
        response_len = strlen(response);
    }
    printf("MESSAGE LENGTH: %d\n", response_len);
    ssize_t bytes_sent = send(connected_sockfd, *response_ptr, response_len, 0);
    if (bytes_sent == -1) {
        perror("Error while sending response");
        exit(1);
    }

    free(file_path);
    free(*response_ptr);
    return 1;
}

char* create_expanded_file_path(struct path* base_dir, struct path* host, struct path* path)
{
    char* file_path = malloc(sizeof(char) * (base_dir->length + host->length + path->length + 1));
    memcpy(file_path, base_dir->path, base_dir->length);
    memcpy(file_path + base_dir->length, host->path, host->length);
    memcpy(file_path + base_dir->length + host->length, path->path, path->length + 1);
    char* expanded_path = realpath(file_path, NULL);
    if (expanded_path == NULL) {
        if (errno == EACCES || errno == ENOENT) {
            return file_path;
        }
        perror("An error occurred while expanding the path");
        exit(1);
    }
    free(file_path);
    return expanded_path;
}

char* create_redirect_path(struct path* path)
{
    char* homepage;
    if (path->path[path->length-1] == '/') {
        homepage = "index.html";
    } else {
        homepage = "/index.html";
    }
    int homepage_len = strlen(homepage);

    char* redirect_path = malloc(sizeof(char) * (path->length + homepage_len + 1));
    memcpy(redirect_path, path->path, path->length);
    memcpy(redirect_path + path->length, homepage, homepage_len + 1);
    return redirect_path;
}

int is_directory(char* path)
{
    struct stat stats;
    int error = stat(path, &stats);
    return !error && S_ISDIR(stats.st_mode);
}

int check_permissions(struct path* base_dir, struct path* host, char* file_path)
{
    int file_path_len = strlen(file_path);
    if (file_path_len < base_dir->length + host->length) {
        return -2;
    }
    int i = 0;
    printf("FILE PATH: %s\n", file_path);
    printf("BASE DIR: %s\n", base_dir->path);
    printf("HOST: %s\n", host->path);
    for (; i < base_dir->length; i++) {
        if (base_dir->path[i] != file_path[i]) {
            return -2;
        }
    }
    for (int j = 0; j < host->length; j++) {
        if (host->path[j] != file_path[i++]) {
            return -2;
        }
    }
    return access(file_path, R_OK);
}


char* get_content_type(struct path* path) 
{
    char* extension;
    for (int i=path->length; i >= 0; i--) {
        if (path->path[i] == '.') {
            extension = path->path + i + 1;
            break;
        }
    }
    if (strncmp("jpg", extension, 3) == 0 || strncmp("jpeg", extension, 4) == 0) {
        return JPEG_TYPE;
    }
    else if (strncmp("png", extension, 3) == 0) {
        return PNG_TYPE;
    }
    else if (strncmp("txt", extension, 3) == 0) {
        return TXT_TYPE;
    }
    else if (strncmp("html", extension, 4) == 0) {
        return HTML_TYPE;
    }
    else if (strncmp("css", extension, 3) == 0) {
        return CSS_TYPE;
    }
    else if (strncmp("pdf", extension, 3) == 0) {
        return PDF_TYPE;
    }
    return DEFAULT_TYPE;
}

int read_file(const char* file_path, char** response_ptr, int size)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Could not open file to read");
        exit(1);
    }
    int offset = strlen(*response_ptr);

    *response_ptr = realloc(*response_ptr, sizeof(char) * (size + offset));
    fread((*response_ptr) + offset, sizeof(char), size, file);
    fclose(file);
    return size + offset;
}

int get_filesize(const char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Could not open file to calculate size");
        exit(1);
    }
    if (fseek(file, 0, SEEK_END)) {
        perror("Could not find the end of the file");
        exit(1);
    }
    int size = ftell(file);
    if (size == -1) {
        perror("An error occured while calculating filesize");
        exit(1);
    }
    if (fclose(file)) {
        perror("Could not close file");
        exit(1);
    }
    return size;
}