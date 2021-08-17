#ifndef PATTERN_FULL_H
#define PATTERN_FULL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

#include "../main.h"
#include "../version.h"
#include "../Device_lib.h"

void pattern_full_init(int HS);

#endif
