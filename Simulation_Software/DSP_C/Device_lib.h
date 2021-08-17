#ifndef DEVICE_LIB_H_
#define DEVICE_LIB_H_

#include <csl_types.h>

#include "global.h"

#define READ_REGISTER(reg)           DEVICE_REGISTER(reg)
#define WRITE_REGISTER(reg, value)   {DEVICE_REGISTER(reg) = value; (*(volatile Uint32 *)(0x70000000)) = 0;  (*(volatile Uint32 *)(0x70000000)); }

extern Uint32 MeaData[MAX_DATAPOINTS_PER_FRAME];
extern Uint32 MonitorData[MONITOR_ARRAY];
extern Uint32 ChannelsPerSweepConfigured;

#define IFB_LED_CONFIG    0x002c
#define IFB_AUX_OUT       0x04b0
#define IFB_AUX_IN        0x04b4
#define IFB_AUX_DIR       0x04b8


#define DSP_INDATA_CTRL0    0x0b00

#define DSPINDATACTRL0_INT_ENABLE 0x0100   // Enable IRQ
#define DSPINDATACTRL0_CLEAR_FIFO 0x0002
#define DSPINDATACTRL0_RESET_FIFO 0x0001

#define DSP_OUTDATA_CTRL    0x0b04
#define DSP_OUTDATA_THR     0x0b14
#define DSP_INDATA_CHANNELS 0x0b18
#define FEEDBACK_REGISTER   0x0480
#define MAILBOX_CTRL        0x0b24

#define DIGITAL_MUX  0x0540 // Digital MUX

#define DSP_INDATA_CONFIG0 0xb80
#define DSP_INDATA_CONFIG1 0xb81
#define DSP_INDATA_CONFIG2 0xb82
#define DSP_INDATA_CONFIG3 0xb83
#define DSP_INDATA_CONFIG4 0xb84
#define DSP_INDATA_CONFIG5 0xb85
#define DSP_INDATA_CONFIG6 0xb86
#define DSP_INDATA_CONFIG7 0xb87
#define DSP_INDATA_CONFIG8 0xb88
#define DSP_INDATA_CONFIG9 0xb89
#define DSP_INDATA_CONFIGa 0xb8a
#define DSP_INDATA_CONFIGb 0xb8b
#define DSP_INDATA_CONFIGc 0xb8c
#define DSP_INDATA_CONFIGd 0xb8d
#define DSP_INDATA_CONFIGe 0xb8e
#define DSP_INDATA_CONFIGf 0xb8f

#define MAILBOX_LASTWRITTEN 0xb28 /// was 0x428 MEA2100
#define MAILBOX_BASE 0x1000
#define MAILBOX_THRSHOLD  0x00
#define MAILBOX_DEADTIME  0x04
#define MAILBOX_AMPLITUDE 0x08
#define MAILBOX_DURATION  0x0c
#define MAILBOX_REPEATS   0x10
#define MAILBOX_STEPSIZE  0x14



void MEA21_init();
void MEA21_enableData();

void SetMonitorSize(int datapoints);
void timer_setperiod(int period);


#endif
