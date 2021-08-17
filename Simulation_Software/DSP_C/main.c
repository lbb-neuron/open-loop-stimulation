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
#include <soc.h>
#include <c6x.h>

#include "main.h"
#include "DSP.h"

char dsp_version[] = "Open Loop 3.0";

void main()
{
    int h,j,status,updating_network;

    int elec_stim_left[8];
    int elec_stim_right[8];
    int empty_stim[8];
    int stim_0[8];
    int stim_1[8];
    int stim_2[8];
    int stim_3[8];

    for(h=0;h<8;h++)
        empty_stim[h] = 0;

    init();
    status = 0;

    // Let software now DSP is ready
    WRITE_REGISTER(0x1000,0x0001);
    led_inform_dsp_ready();


    while(status != 0x0008)
    {
        while(READ_REGISTER(0x1000) == 0x0001);
        status           = READ_REGISTER(0x1000);
        updating_network = READ_REGISTER(0x1004);
        if(status == 0x0002)
        {
            int HS = (updating_network & 0xff00 >> 8);
            updating_network &= 0xff;
            led_channel(LED_OFF,LED_ON);
            stimulation_pattern_lower_update(HS,updating_network);
        }
        else if(status == 0x0003)
        {
            int HS = (updating_network & 0xff00 >> 8);
            updating_network &= 0xff;
            led_channel(LED_OFF,LED_ON);
            stimulation_pattern_upper_update(HS,updating_network);
        }
        else if(status == 0x0004)
        {
            int HS = (updating_network & 0xff00 >> 8);
            updating_network &= 0xff;
            led_channel(LED_ON,LED_OFF);
            stimulation_probability_update(HS,updating_network);
        }
        else if(status == 0x0010)
        {
            int length    = (updating_network & 0xefff0000 >> 16);
            int amplitude = updating_network & 0xefff;
            led_channel(LED_ON,LED_ON);
            setup_all_pulses(amplitude,length);
        }
        else if(status == 0x0020)
        {
            inter_spike_period = updating_network & 0x00ffffff; // Must be smaller than 16777215 us
            led_channel(LED_ON,LED_ON);
        }
        else
        {
            led_channel(LED_OFF,LED_OFF);
        }
        WRITE_REGISTER(0x1000,0x0001);
    }
    epic_time = 0;

    while(1)
    {
        // Get first stimulus
        for(h=0;h<8;h++)
        {
            get_stimulation_pattern(h,0, &elec_stim_left[h], &elec_stim_right[h]);
            transformElectrodes(elec_stim_left[h], elec_stim_right[h], &stim_0[h], &stim_1[h], &stim_2[h], &stim_3[h]);
        }
        led_channel(LED_OFF,LED_OFF);

        while(epic_time < period_us/2)
        {
            status = READ_REGISTER(0x1000);
            updating_network = READ_REGISTER(0x1004);

            if(status != 0x0001)
            {
                // This procedure (if statement) takes roughly 280 us +- 10 us
                WRITE_REGISTER(IFB_AUX_OUT,2);

                // Update stimulation patterns/probabilities, if applicable
                if(status == 0x0002)
                {
                    int HS = (updating_network >> 8) & 0x00ff;
                    updating_network &= 0xff;
                    led_channel(LED_OFF,LED_ON);
                    stimulation_pattern_lower_update(HS,updating_network);
                }
                else if(status == 0x0003)
                {
                    int HS = (updating_network >> 8) & 0x00ff;
                    updating_network &= 0xff;
                    led_channel(LED_OFF,LED_ON);
                    stimulation_pattern_upper_update(HS,updating_network);
                }
                else if(status == 0x0004)
                {
                    int HS = (updating_network >> 8) & 0x00ff;
                    updating_network &= 0xff;
                    led_channel(LED_ON,LED_OFF);
                    stimulation_probability_update(HS,updating_network);
                }
                else if(status == 0x0010)
                {
                    WRITE_REGISTER(0x1608,updating_network);
                    int length    = (updating_network & 0xefff0000) >> 16;
                    int amplitude = updating_network & 0x0000efff;
                    led_channel(LED_ON,LED_ON);
                    setup_all_pulses(amplitude,length);
                }
                else if(status == 0x0020)
                {
                    inter_spike_period = updating_network & 0x00ffffff; // Must be smaller than 16777215 us
                    led_channel(LED_ON,LED_ON);
                    WRITE_REGISTER(0x1610,inter_spike_period);
                }
                WRITE_REGISTER(0x1004,period_us - epic_time);
                WRITE_REGISTER(0x1000,0x0001); // Let PC know copying is finished

                WRITE_REGISTER(IFB_AUX_OUT,0);
            }
        }

        // Do the actual stimulation
        for(j=0;j<num_stim;j++)
        {
            //while(epic_time < period_us + (j-1) * inter_spike_period + inter_spike_period/2);
            while(epic_time < period_us + j * inter_spike_period);
            led_status(LED_ON);
            WRITE_REGISTER(IFB_AUX_OUT,1);
            stimulation_trigger(stim_0, stim_1, stim_2, stim_3);
            int temp_time = epic_time;
            while(temp_time+stim_length>epic_time);
            for(h=0;h<8;h++)
                stimulation_clear();
            if(j+1 < num_stim)
            {
                for(h=0;h<8;h++)
                {
                    get_stimulation_pattern(h,j+1, &elec_stim_left[h], &elec_stim_right[h]);
                    transformElectrodes(elec_stim_left[h], elec_stim_right[h],  &stim_0[h], &stim_1[h], &stim_2[h], &stim_3[h]);
                }
            }
        }
        WRITE_REGISTER(IFB_AUX_OUT,0);
        led_status(LED_OFF);
        epic_time   -= period_us;
    }
}


