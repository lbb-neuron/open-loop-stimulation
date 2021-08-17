#ifndef LED_H
#define LED_H

#include <stdio.h>

#include "../main.h"
#include "../version.h"
#include "../Device_lib.h"

#define LED_ON  1
#define LED_OFF 0

void led_init();
void led_status(unsigned int status);
void led_channel(unsigned int chan1, unsigned int chan2);
void led_inform_dsp_ready();

#endif
