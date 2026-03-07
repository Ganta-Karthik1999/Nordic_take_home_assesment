#include"timer.h"
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include "socket_utils.h"

extern volatile uint64_t odd;
extern volatile uint64_t even;
extern volatile sig_atomic_t running;
extern pthread_mutex_t count_mutex;
extern int seconds;

/**
 * @brief Periodic timer thread that sends odd/even counts every 60 seconds.
 *
 * This function runs in a dedicated thread and is responsible for
 * periodically reporting the number of odd and even values received
 * from clients. Every 60 seconds it:
 *  - Sends a POST request containing the current odd and even counts.
 *  - Resets the counters for the next reporting window.
 *
 * The thread continues running as long as the global `running` flag
 * remains set. A mutex is used when resetting the counters to ensure
 * thread-safe access since multiple client threads may update them.
 *
 * @param arg Unused thread argument.
 *
 * @return NULL when the thread exits.
 */

void *timer_thread_func(void *arg)
{
    (void)arg;

    while (running) {
        sleep(TIMER_INTERVAL_SECONDS);
        send_post_request(odd, even);
        pthread_mutex_lock(&count_mutex);
        odd = 0;
        even = 0;
        pthread_mutex_unlock(&count_mutex);

    }

    return NULL;
}