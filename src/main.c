#include "socket_utils.h"

volatile uint64_t odd = 0;
volatile uint64_t even = 0;
volatile sig_atomic_t running = 1;
volatile int active_connections = 0;  
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{    
    int sockfd, clientfd, status, opt = 1;
    struct sockaddr_in addr, client_addr;
    char text[255];
    socklen_t addr_len;
    uint64_t rand_value;
    pthread_t thread_id;


    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;  
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return sockfd;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4242);
    inet_aton("127.0.0.1", &addr.sin_addr);
    // addr.sin_addr.s_addr = INADDR_ANY;

    status = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (status) {
        perror("bind");
        close(sockfd);
        return status;
    }

    status = listen(sockfd, 5);
    if (status) {
        perror("listen");
        close(sockfd);
        return status;
    }
    curl_global_init(CURL_GLOBAL_ALL);
    
    long last_time = get_time_ms();
    int interval_ms = 60000;  
    curl_global_init(CURL_GLOBAL_ALL);
    while (running == 1) {
        long now = get_time_ms();
       
        if (now - last_time >= interval_ms) {
            printf("Timer fired!\n");
            pthread_mutex_lock(&count_mutex);
            send_post_request(odd, even);
            odd=0;
            even=0;
            pthread_mutex_unlock(&count_mutex);
            last_time = now;
        }
       
       
        addr_len = sizeof(client_addr);
        clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);  
        if (clientfd < 0) {
            if (errno == EINTR) {
                continue;  
            }
            perror("accept");
            return status;
        }
        pthread_mutex_lock(&count_mutex);
        active_connections++;
        pthread_mutex_unlock(&count_mutex);


        printf("Client connected - Port: %d, IP: %s\n",
               ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr));
        printf("Active connections: %d\n", active_connections);
        printf("=====================================================\n");

        int *client_fd_ptr = malloc(sizeof(int));
        if (client_fd_ptr == NULL) {
            perror("malloc");
            close(clientfd);
            continue;
        }
        *client_fd_ptr = clientfd;

        if (pthread_create(&thread_id, NULL, handle_client, client_fd_ptr) != 0) {
            perror("pthread_create");
            close(clientfd);
            free(client_fd_ptr);
            continue;
        }

        pthread_detach(thread_id);

 }

    curl_global_cleanup();
    close(sockfd);
    printf("\nShutting down server...\n");
    printf("Server stopped.\n");
    return 0;
}