#ifndef __SOCKET_UTILS_H__
#define __SOCKET_UTILS_H__

#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "json.h"
#include "post.h"
#include "counter.h"
#include "common.h"


extern volatile sig_atomic_t running;
extern volatile int active_connections;


int socket_init(int *sockfd, int opt);

// Accept the Client connection
int accept_client(int sockfd);

// 
void handle_sigint(int sig);
void *handle_client(void *arg);
void socket_close();

#endif