#ifndef __TIMER_H__
#define __TIMER_H__

#include<stdio.h>
#include <time.h>


long get_time_ms(void);
void process_timer_event(long *last_time, int interval_ms);

#endif