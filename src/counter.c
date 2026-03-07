#include"counter.h"

void check_odd_or_even(uint64_t rand_value) {
    pthread_mutex_lock(&count_mutex);
    if (rand_value % 2 == 0) {
        even = even + 1;
    } else {
        odd = odd + 1;
    }
    pthread_mutex_unlock(&count_mutex);
}
