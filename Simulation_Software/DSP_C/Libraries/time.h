#ifndef TIME_H
#define TIME_H

#include <stdio.h>

#include <cslr_pllc.h>
#include <cslr_gpio.h>
#include <cslr_emifa.h>
#include <cslr_ddr2.h>
#include <cslr_dev.h>
#include <cslr_intc.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <cslr_tmr.h>
#include <soc.h>

#include <math.h>
#include <c6x.h>

extern void intcVectorTable(void);
interrupt void interrupt7(void);

#include "../main.h"
#include "../version.h"
#include "../Device_lib.h"

#define MAX_EPIC_TIME 3000000000

void timer_delay_us(Uint32 timeInMicroseconds);
void timer_delay_ms(Uint32 timeInMicroseconds);
void timer_short_wait(void);

#endif
