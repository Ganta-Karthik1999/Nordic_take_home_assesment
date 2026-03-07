#ifndef COUNTER_H
#define COUNTER_H

#include"common.h"
#include<json-c/json.h>
#include<pthread.h>

extern volatile uint64_t odd;
extern volatile uint64_t even;
extern pthread_mutex_t count_mutex;

/**
 * @brief Updates odd and even counters based on the received value.
 * **/
void check_odd_or_even(uint64_t rand_value) ;

#endif