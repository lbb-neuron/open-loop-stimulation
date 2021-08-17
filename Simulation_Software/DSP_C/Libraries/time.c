#include "time.h"

volatile long epic_time;

void timer_delay_us(Uint32 timeInMicroseconds) // Wait in us using isr
{
    Uint32 startTime;
    startTime = epic_time;
    while(((epic_time - startTime) < timeInMicroseconds) && (epic_time >= startTime));
}

void timer_delay_ms(Uint32 timeInMilliseconds) // Wait in us using isr
{
    Uint32 i;
    for(i=0;i<timeInMilliseconds;i++)
        timer_delay_us(1000);
}

void timer_short_wait(void)
{
    volatile int i;
    for(i=0;i<150;i++);
}
