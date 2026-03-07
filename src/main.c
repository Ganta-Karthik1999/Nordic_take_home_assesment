#include "socket_utils.h"
#include "timer.h"
#include"common.h"


volatile uint64_t odd = 0;
volatile uint64_t even = 0;
volatile sig_atomic_t running = 1;
volatile int active_connections = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;


int main()
{  

    int sockfd =0, opt = 1;
   
    //  Initilazing the TCP Socket check the socket_utils.c file for full functionality
    if (socket_init(&sockfd, opt) != 0) {
        fprintf(stderr, "Socket init failed\n");
        return 1;
    }
    
    //  Timer thread ID
    pthread_t timer_thread;

    //  Creating a Timer thread to interrupt every 60 secounds
    if (pthread_create(&timer_thread, NULL, timer_thread_func, NULL) != 0) {
        perror("pthread_create timer_thread");
        curl_global_cleanup();
        close(sockfd);
        return 1;
    }
    //  detaching the timer thread
    pthread_detach(timer_thread);
   
    //  Looping untill keyboard interrupt Cntrl+C
    while (running == 1) {
        // accepting the Client connections
        int rc = accept_client(sockfd);
        if (rc < 0) {
            break;
        }      
      
 }
    // Closing the Socket
    socket_close(&sockfd);
    return 0;
}