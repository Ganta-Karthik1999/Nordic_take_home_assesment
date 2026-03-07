#include"timer.h"
#include "socket_utils.h"

/**
 * @brief Function uses system clock to return time in milliseconds
 * @return Time in milliseconds
 */
long get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void process_timer_event(long *last_time, int interval_ms)
{
    long now = get_time_ms();
    // printf("\r%d",now);
    if (now - *last_time >= interval_ms) {
        // printf("Timer fired!\n");

        pthread_mutex_lock(&count_mutex);
        send_post_request(odd, even);
        odd = 0;
        even = 0;
        pthread_mutex_unlock(&count_mutex);
        printf("odd=%d even=%d\n",odd,even);
        *last_time = now;
    }
}