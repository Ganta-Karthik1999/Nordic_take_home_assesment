#ifndef __SOCKET_UTILS_H__
#define __SOCKET_UTILS_H__

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include<string.h>
#include"json.h"
#include"post.h"
#include"counter.h"

extern volatile sig_atomic_t running;
extern volatile int active_connections;


int accept_client(int sockfd);
int socket_init(int *sockfd, int opt);
void handle_sigint(int sig);
void *handle_client(void *arg);

#endif