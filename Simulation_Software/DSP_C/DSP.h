#ifndef FOO_H
#define FOO_H

#include <stdio.h>
#include <string.h>

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
extern int num_stim;
extern int circuit_type[8];
extern int random_pattern;
extern long long int period_us;
extern int inter_spike_period;
extern Uint32 counter_stimulation;
extern volatile int stim_length;
extern int multi_pattern;

#include "main.h"
#include "version.h"
#include "Device_lib.h"

#include "Libraries/time.h"
#include "Libraries/stimulation.h"
#include "Libraries/led.h"

#include "Libraries/stim_pattern.h"
#include "Libraries/pattern_full.h"
#include "Libraries/pattern_circle.h"

void reset_mailbox(void);
void init(void);

#endif
