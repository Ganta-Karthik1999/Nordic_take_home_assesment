#ifndef POST_H
#define POST_H

#include "common.h"
#include <curl/curl.h>
#include <string.h>
// #include "socket_utils.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, char *response_buffer);
void send_post_request(uint64_t odd, uint64_t even);


#endif