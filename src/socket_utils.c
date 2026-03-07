#include "socket_utils.h"


/**
 * @brief Singal handler for SIGINT(Interrupt Signal)
 * @param sig The signal number (SIGINT)
 * @return void
 */
void handle_sigint(int sig) {
    running = 0;
}

int socket_init(int *sockfd, int opt)
{
    int status;
    struct sockaddr_in addr;
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction");
        return -1;
    }

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        perror("socket");
        return -1;
    }

    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(*sockfd);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4242);

    if (inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton failed\n");
        close(*sockfd);
        return -1;
    }

    if (bind(*sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(*sockfd);
        return -1;
    }

    if (listen(*sockfd, 5) < 0) {
        perror("listen");
        close(*sockfd);
        return -1;
    }

    if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
        fprintf(stderr, "curl_global_init failed\n");
        close(*sockfd);
        return -1;
    }

    return 0;
}



// Thread function to handle each client
void *handle_client(void *arg) {
    int clientfd = *((int *)arg);
    free(arg);  
    char text[255];
    uint64_t rand_value;
    int status;

    memset(text, 0, sizeof(text));
    status = recv(clientfd, text, sizeof(text) - 1, 0);
    if (status > 0) {
        if (parse_json_data(text, &rand_value) == 0) {
            check_odd_or_even(rand_value);
        }
    }

     pthread_mutex_lock(&count_mutex);

    // printf("Thread exiting: %lu\n",
    //        (unsigned long)pthread_self());

    pthread_mutex_unlock(&count_mutex);
    close(clientfd);
    return NULL;
}




int accept_client(int sockfd)
{
    int clientfd;
    pthread_t thread_id;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
    if (clientfd < 0) {
        if (errno == EINTR) {
            return 1;   // interrupted, caller can continue
        }
        perror("accept");
        return -1;
    }

    int *client_fd_ptr = malloc(sizeof(int));
    if (client_fd_ptr == NULL) {
        perror("malloc");
        close(clientfd);

        return 1;
    }

    *client_fd_ptr = clientfd;;
    if (pthread_create(&thread_id, NULL, handle_client, client_fd_ptr) != 0) {
        perror("pthread_create");
        close(clientfd);
        free(client_fd_ptr);

        return 1;
    }

    pthread_detach(thread_id);
    return 0;
}

