#ifndef GLOBAL_H_
#define GLOBAL_H_

// #define USE_SIMULATOR
#define USE_MAILBOX_IRQ

#include <csl.h>

#include "Device_config.h"

#define I2C_BUFFER_SIZE 512

#define MONITOR_ARRAY   		8

#define STIMULUS_VECTOR_SIZE 	10
#define SENDDATA_PER_FRAME 		64

#define DOWNSAMPLE         	1
#define FRAMES_PER_LOOP    	1

#define ELECTRODES_PER_REGISTER 32
#define TRIGGER_ID_HS1         0x0140
#define TRIGGER_SET_EVENT_HS1  0x0110

extern Int32 adc_intern[HS1_CHANNELS + IF_CHANNELS];

extern int threshold;
extern int deadtime;

extern int StimAmplitude;
extern int StimDuration;
extern int StimRepeats;
extern int StimStepsize;

extern volatile long epic_time;
extern int SCU1_BEING_USED;
extern int SCU2_BEING_USED;

extern Uint32 StimulusEnable[HS1_CHANNELS / ELECTRODES_PER_REGISTER];
extern Uint32 elec_config[HS1_CHANNELS / ELECTRODES_PER_REGISTER];
extern Uint32 DAC_select[HS1_CHANNELS / (ELECTRODES_PER_REGISTER/2)];

#endif /*GLOBAL_H_*/
