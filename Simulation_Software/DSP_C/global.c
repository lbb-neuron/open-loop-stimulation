#include "global.h"

Int32 adc_intern[HS1_CHANNELS + IF_CHANNELS];
#pragma DATA_ALIGN(adc_intern, 8);

int threshold;
int deadtime;

int StimAmplitude;
int StimDuration;
int StimRepeats;
int StimStepsize;

Uint32 StimulusEnable[HS1_CHANNELS / ELECTRODES_PER_REGISTER];
Uint32 elec_config[HS1_CHANNELS / ELECTRODES_PER_REGISTER];
Uint32 DAC_select[HS1_CHANNELS / (ELECTRODES_PER_REGISTER/2)];
