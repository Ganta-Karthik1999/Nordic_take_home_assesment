#include"timer.h"
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "socket_utils.h"

extern volatile uint64_t odd;
extern volatile uint64_t even;
extern volatile sig_atomic_t running;
extern pthread_mutex_t count_mutex;

void *timer_thread_func(void *arg)
{
    (void)arg;

    while (running) {
        sleep(60);
        send_post_request(odd, even);
        pthread_mutex_lock(&count_mutex);
        odd = 0;
        even = 0;
        pthread_mutex_unlock(&count_mutex);

    }

    return NULL;
}