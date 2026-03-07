#include "socket_utils.h"
#include "timer.h"

volatile uint64_t odd = 0;
volatile uint64_t even = 0;
volatile sig_atomic_t running = 1;
volatile int active_connections = 0;  
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{    
    int sockfd =0, opt = 1;
    if (socket_init(&sockfd, opt) != 0) {
        fprintf(stderr, "Socket init failed\n");
        return 1;
    }
    long last_time = get_time_ms();
    int interval_ms = 60000;  

    while (running == 1) {
        process_timer_event(&last_time, interval_ms);
        
        int rc = accept_client(sockfd);
        if (rc < 0) {
            break;
        }
       
      
 }

    curl_global_cleanup();
    close(sockfd);
    printf("\nShutting down server...\n");
    printf("Server stopped.\n");
    return 0;
}