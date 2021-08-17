#include "DSP.h"

void reset_mailbox(void)
{
    volatile int i=0;
    for(i=0x0;i<0xfff;i+=4)
    {
        WRITE_REGISTER(i+0x1000,0);
    }
}

int SCU1_BEING_USED = 0;
int SCU2_BEING_USED = 0;

void init(void)
{
    int i,circuit_values;

    reset_mailbox();

    // Use SCU 1
    SCU1_BEING_USED = 1;

    MEA21_init();

    WRITE_REGISTER(IFB_AUX_DIR, 0x3); // set AUX 1 and 2 as output
    WRITE_REGISTER(IFB_AUX_OUT, 0x0); // set AUX 1 to value 0

    MEA21_enableData();

    WRITE_REGISTER(0x1000,2);         // Let c# program know, that software is ready to receive the parameters for the SCU types
    while(READ_REGISTER(0x1000));     // Wait until all the important parameters have been sent, then read them out
    SCU1_BEING_USED     = READ_REGISTER(0x1004)&1;
    SCU2_BEING_USED     = READ_REGISTER(0x1008)&1;
    num_stim            = READ_REGISTER(0x100c)&0x1f;
    random_pattern      = READ_REGISTER(0x1010)&1;
    circuit_values      = READ_REGISTER(0x1014);
    for(i=0;i<8;i++)
        circuit_type[i] = (circuit_values >> 4*i)&0xf;
    period_us           = READ_REGISTER(0x1018);
    inter_spike_period  = READ_REGISTER(0x101c);
    multi_pattern       = READ_REGISTER(0x1020);

    stimulation_init(SCU1_BEING_USED,SCU2_BEING_USED);
    stim_pattern_init();
    led_init();

    epic_time = 0;

    WRITE_REGISTER(0x1000,1); // Give the code the sign that everything got set up properly
}
