#ifndef __SOCKET_UTILS_H__
#define __SOCKET_UTILS_H__

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include<json-c/json.h>
#include <time.h>
#include <curl/curl.h>
#include <pthread.h>

extern volatile uint64_t odd;
extern volatile uint64_t even;
extern volatile sig_atomic_t running;
extern pthread_mutex_t count_mutex;
extern volatile int active_connections;

void process_timer_event(long *last_time, int interval_ms);
int accept_and_spawn_client(int sockfd);
int socket_init(int *sockfd, int opt);
long get_time_ms(void);
void handle_sigint(int sig);
void send_post_request(uint64_t odd, uint64_t even);
int parse_json_data(const char *json_str, uint64_t *rand_value);
size_t write_callback(char *ptr, size_t size, size_t nmemb, char *response_buffer);
void *handle_client(void *arg);
void check_odd_or_even(uint64_t rand_value);

#endif