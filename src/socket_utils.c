#include "socket_utils.h"


/**
 * @brief Singal handler for SIGINT(Interrupt Signal)
 * @param sig The signal number (SIGINT)
 * @return void
 */
void handle_sigint(int sig) {
    running = 0;
}

/**
 * @brief Initializes the TCP server socket and required system resources.
 *
 * This function performs all initialization required for the server:
 *  - Registers a SIGINT handler to allow graceful shutdown.
 *  - Creates a TCP socket using IPv4.
 *  - Enables the SO_REUSEADDR option for the socket.
 *  - Binds the socket to localhost (127.0.0.1) on port 4242.
 *  - Places the socket into listening mode to accept incoming connections.
 *  - Initializes the global CURL environment required for HTTP POST requests.
 *
 * @param sockfd Pointer to an integer where the created socket file descriptor
 *               will be stored.
 * @param opt    Socket option value used for SO_REUSEADDR configuration.
 *
 * @return 0  Initialization successful.
 * @return -1 Initialization failed (error message printed using perror/printf).
 */

int socket_init(int *sockfd, int opt)
{
    int status;
    struct sockaddr_in addr;
    struct sigaction sa;

    //  Calls the handle_signal function initialized above
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


/**
 * @brief Thread routine to handle a single client connection.
 *
 * This function is executed by a worker thread created for each
 * incoming client connection. It performs the following steps:
 *  - Retrieves the client socket file descriptor from the thread argument.
 *  - Receives a JSON message sent by the client.
 *  - Parses the JSON data to extract the random value.
 *  - Updates the odd/even counters based on the received value.
 *  - Closes the client connection before terminating the thread.
 *
 * @param arg Pointer to dynamically allocated memory containing
 *            the client socket file descriptor.
 *
 * @return NULL when the thread completes execution.
 */

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

    close(clientfd);
    return NULL;
}



/**
 * @brief Accepts the client connection.
 * - check's if the accept is sucessful or not.
 * - create a client file descriptor and allocate memory block
 * - create a new thread everytime a new client connection is initiates 
 * - if connections is sucessfull then  the Handle client will be called
 * 
 * @param Sockfd Pointer to an integer where the created socket file descriptor
 * @return 0 Sucessfully accepted connection
 * @return 1 connection unsucessfull
 * 
 * **/
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

/**
 * @brief Cleanly shuts down the server and releases allocated resources.
 *
 * This function performs the necessary cleanup before terminating
 * the server application. It:
 *  - Cleans up the global CURL environment used for HTTP requests.
 *  - Closes the server socket file descriptor.
 *  - Prints shutdown messages to indicate the server has stopped.
 *
 * @param sockfd Pointer to the server socket file descriptor that
 *               should be closed.
 *
 * @return void
 */

void socket_close(int *sockfd){
    curl_global_cleanup();
    close(*sockfd);
    printf("\nShutting down server...\n");
    printf("Server stopped.\n");
}