#include"counter.h"

/**
 * @brief Updates odd and even counters based on the received value.
 *
 * This function determines whether the provided random value is
 * odd or even and increments the corresponding counter. A mutex
 * lock is used to ensure thread-safe access to the shared counters,
 * since multiple client threads may update them concurrently.
 *
 * @param rand_value The random value received from the client.
 *
 * @return void
 */


void check_odd_or_even(uint64_t rand_value) {
    pthread_mutex_lock(&count_mutex);
    if (rand_value % 2 == 0) {
        even = even + 1;
    } else {
        odd = odd + 1;
    }
    pthread_mutex_unlock(&count_mutex);
}
