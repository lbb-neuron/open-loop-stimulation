#ifndef STIMULATION_H
#define STIMULATION_H

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

#define PI 3.141593

#include <math.h>
#include <c6x.h>

extern void intcVectorTable(void);

void ClearChannel(int channel, int segment);
void stimulation_init(int HS1_used, int HS2_used);
void setup_all_pulses(int amplitude, int length);
void UploadBiphaseRect(int channel, int segment, int amplitude, int duration, int repeats);
void SetSegment(int scu, int channel, int segment);
int  AddDataPoint(int channel, int duration, int value);
void SetupTrigger();
void stimulation_trigger(int *stim_0, int *stim_1, int *stim_2, int *stim_3);
void stimulation_single_electrode(int SCU, int HS, int electrode);
void stimulation_clear(void);
void transformElectrodes(int elec_stim_left, int elec_stim_right, int *stim_0, int *stim_1, int *stim_2, int *stim_3);
void set_stimulation_registers(int *stim_0, int *stim_1, int *stim_2, int *stim_3);

#endif
