#ifndef POST_H
#define POST_H

#include "common.h"
#include <curl/curl.h>
#include <string.h>


/**
 * @brief Function callback that captures responses
 * 
 * **/
size_t write_callback(char *ptr, size_t size, size_t nmemb, char *response_buffer);

/**
 * @brief Function to send POST request to paste.c-net.org

 * 
 * **/
void send_post_request(uint64_t odd, uint64_t even);


#endif