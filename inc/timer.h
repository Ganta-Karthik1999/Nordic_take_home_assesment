#ifndef __TIMER_H__
#define __TIMER_H__

#include"common.h"
#include <time.h>

//  Time in seconds before each post response
#define TIMER_INTERVAL_SECONDS 60

void *timer_thread_func(void *arg);
#endif