#ifndef STIM_PATTERN_H
#define STIM_PATTERN_H

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

#define MAX_NUMBER_PROBABILITIES 256 // Max number of probabilities
#define MAX_NUMBER_PATTERNS      128 // Max number of different patterns at a given time per network (must be <= 256)
#define MAX_NUMBER_CIRCUITS       16 // Max number of circuits per Headstage (must be divisable by 4)
#define MAX_NUMBER_STIMULATIONS   16 // Max number of stimuli per pattern
// It must hold that: 8*MAX_NUMBER_PATTERNS*MAX_NUMBER_STIMULATIONS == 2*32*256. This is due to the fact that with each message-package, we can only send 32*256 bits and there are 2 such package types.

struct Pattern
{
    volatile Uint8  probabilites[MAX_NUMBER_PROBABILITIES];                 // Defines the probability distribution. In each probability one should save what pattern should be chosen
    volatile Uint8  patterns[MAX_NUMBER_PATTERNS][MAX_NUMBER_STIMULATIONS]; // Saves each pattern.
    Uint8           electrodes[60];                                         // Location of electrodes (0-59)
    Uint8           current_pattern;                                        // This feature is here to know which one is currently the pattern used (patterns[probabilities[current_pattern]])
};

extern int circuit_type[8];
extern int num_stim;
extern int num_circuits[8];
extern int num_electrodes[8];
extern struct Pattern stim_patterns[8][MAX_NUMBER_CIRCUITS];

void stim_pattern_init();
void set_network_type(int HS, int type);
void get_single_stimulation_pattern(int HS, int circuit, int position, int *elec_stim_left, int *elec_stim_right);
void get_stimulation_pattern(int HS, int position, int *elec_stim_left, int *elec_stim_right);
void stimulation_pattern_lower_update(int HS, int circuit);
void stimulation_pattern_upper_update(int HS, int circuit);
void stimulation_probability_update(int HS, int circuit);

#endif
