#include "../main.h"
#include "../version.h"
#include "../Device_lib.h"

#include "stimulation.h"

int electrode_encoding[60] = {6,7,9,11,14,15,18,20,22,23,
                              4,3,5,10,13,16,19,24,26,25,
                              1,2,0,8,12,17,21,29,27,28,
                              58,57,59,51,47,42,38,30,32,31,
                              55,56,54,49,46,43,40,35,33,34,
                              53,52,50,48,45,44,41,39,37,36};

volatile int stim_length; // Gets set in setup_all_pulses

void stimulation_init(int HS1_used, int HS2_used)
{
    int i;
    setup_all_pulses(1000,240);
    SetupTrigger();

    //Line below might need to be changed if using multiple SCUs.
    WRITE_REGISTER(0x0540, 0x42080200);   // Sideband data 0 HS 1 (0x42)  bit 8  to Digital Out (0x02) bit 0

    WRITE_REGISTER(0x9c51,0x0); // Electrode Config ID source select = Trigger 0
    int mux  = 0;
    int mode = 0;
    for(i=0;i<16;i++)
        mux  |= (1 << 2*(i));
    for(i=0;i<32;i++)
        mode |= 1 << i;
    if(HS1_used)
    {
        for(i=0;i<16;i++)
            WRITE_REGISTER(0x9cd0+i,mux);   // Set all the multiplexer to channel 1
        for(i=0;i<8;i++)
            WRITE_REGISTER(0x9c70+i,0);     // Set all the modes to automatic
        for(i=0;i<4;i++)
            WRITE_REGISTER(0x9400+i,mode);
        for(i=0;i<8;i++)
            WRITE_REGISTER(0x8400+i,0);//0xffffffff);

        WRITE_REGISTER(0x9480,0x80000007);
        WRITE_REGISTER(0x94a0,0x80000007);
    }
}

void setup_all_pulses(int amplitude, int length)
{
    int i;
    stim_length = length/40;
    for(i=0;i<18;i+=4)
    {
        UploadBiphaseRect(i,   0,  amplitude, length, 0);
        UploadBiphaseRect(i+2, 0, -amplitude, length, 0);
    }
    stim_length = stim_length*40;
}

void UploadBiphaseRect(int channel, int segment, int amplitude, int duration, int repeats)
{
    int vectors_used;

    vectors_used = 0;
    duration /= 40; // This makes the total stim length in us
    ClearChannel(channel, segment);
    vectors_used += AddDataPoint(channel, duration, 0x8000 - amplitude);
    vectors_used += AddDataPoint(channel, duration, 0x8000 + amplitude);
    vectors_used += AddDataPoint(channel, 10, 0x8000);

    vectors_used = 0;
    ClearChannel(channel+1, segment);
    // Shift of 40 us is necessary due to internal DSP/SCU/HS pipeline
    vectors_used += AddDataPoint(channel+1, 2, 0x0019);          // bit 1,3,4
    vectors_used += AddDataPoint(channel+1, 2*duration, 0x0119); // bit 1,3,4 and 8
    vectors_used += AddDataPoint(channel+1, 2, 0x0019);          // bit 1,3,4
}

void SetSegment(int scu, int channel, int segment)
{
    if(scu == 1)
    {
        Uint32 SegmentReg = 0x9200 + (channel * 8);
        WRITE_REGISTER(SegmentReg, segment);
    }
    else if (scu == 2)
    {
        Uint32 SegmentReg = 0xd200 + (channel * 8);
        WRITE_REGISTER(SegmentReg, segment);
    }
}

void ClearChannel(int channel, int segment)
{
    Uint32 ClearReg = 0x9203 + (channel * 8);
    SetSegment(0,channel, segment);
    WRITE_REGISTER(ClearReg, 0);        // Any write to this register clears the channeldata
}

int AddDataPoint(int channel, int duration, int value)
{
    int vectors_used = 0;
    int Vector;
    Uint32 ChannelReg = 0x9f80 + channel * 1;

    if (duration > 1000)
    {
        Vector = 0x04000000 | (((duration / 1000) - 1) << 16) | (value & 0xffff);
        WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
        duration %= 1000;
        vectors_used++;
    }

    if (duration > 0)
    {
        Vector = ((duration - 1) << 16) | (value & 0xffff);
        WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
        vectors_used++;
    }

    return vectors_used;
}

void SetupTrigger()
{
    int i;

    WRITE_REGISTER(0x0100, 0x1);  // Enable Trigger Packets

    for (i = 0; i < 18; i++)
    {
        WRITE_REGISTER(0x0140+i*4, 0x0);    // Setup Trigger
        WRITE_REGISTER(0x9600+i, (4 << 1) | 1);
        WRITE_REGISTER(0x9700+i, 1);  // Trigger Repeat: only once
    }
}
void stimulation_trigger(int *stim_0, int *stim_1, int *stim_2, int *stim_3)
{
    set_stimulation_registers(stim_0, stim_1, stim_2, stim_3);

    WRITE_REGISTER(0x0110, 0xfff);
    while(READ_REGISTER(0x0110)& 0xfff);
    //stimulation_clear(); // Important. Otherwise an electrode cannot be stimulated in two consecutive triggers.
}

void stimulation_single_electrode(int SCU, int HS, int electrode)
{
    int i,j;
    //TODO: mux only supports SCU1 for now
    int mux[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    for(i=0;i<240;i++)
    {
        int mux_HS  = i/60;
        int sub_reg = (i%60)/16;
        int sub_ele = i - mux_HS*60 - sub_reg*16;
        if(HS == mux_HS && electrode == sub_reg*16+sub_ele)
            mux[HS][sub_reg] |= 1 << (2*sub_ele);
        else
            mux[HS][sub_reg] |= 0 << (2*sub_ele);
    }
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
            WRITE_REGISTER(0x9cd0+i*4+j,mux[i][j]);

    for(i=0;i<8;i++)
    {
        WRITE_REGISTER(0x9ca0+i,0xffffffff);
        WRITE_REGISTER(0xdca0+i,0xffffffff);
    }
}

void stimulation_clear(void)
{
    int i;
    for(i=0;i<8;i++)
    {
        WRITE_REGISTER(0x9cd0+i, 0);
        WRITE_REGISTER(0xdcd0+i, 0);
    }
    for(i=0;i<8;i++)
    {
        WRITE_REGISTER(0x9ca0+i, 0);
        WRITE_REGISTER(0xdca0+i, 0);
    }
}

void transformElectrodes(int elec_stim_left, int elec_stim_right, int *stim_0, int *stim_1, int *stim_2, int *stim_3)
{
    int i;

    *stim_0  = 0;
    *stim_1  = 0;
    *stim_2  = 0;
    *stim_3  = 0;
    for(i=0;i<30;i++)
    {
        int elec;
        int sub_reg;
        int sub_ele;
        if(elec_stim_left & (1<<i))
        {
            elec    = electrode_encoding[i];
            sub_reg = elec/16;
            sub_ele = elec%16;
            switch(sub_reg)
            {
                case 0: *stim_0 = *stim_0 | (1 << (2*sub_ele)); break;
                case 1: *stim_1 = *stim_1 | (1 << (2*sub_ele)); break;
                case 2: *stim_2 = *stim_2 | (1 << (2*sub_ele)); break;
                case 3: *stim_3 = *stim_3 | (1 << (2*sub_ele)); break;
            }
        }
        if(elec_stim_right & (1<<i))
        {
            elec    = electrode_encoding[i+30];
            sub_reg = elec/16;
            sub_ele = elec%16;
            switch(sub_reg)
            {
                case 0: *stim_0 = *stim_0 | (1 << (2*sub_ele)); break;
                case 1: *stim_1 = *stim_1 | (1 << (2*sub_ele)); break;
                case 2: *stim_2 = *stim_2 | (1 << (2*sub_ele)); break;
                case 3: *stim_3 = *stim_3 | (1 << (2*sub_ele)); break;
            }
        }
    }
}

void set_stimulation_registers(int *stim_0, int *stim_1, int *stim_2, int *stim_3)
{
    volatile int i;
    for(i=0;i<4;i++)
    {
        // Set the mux signal for SCU1
        WRITE_REGISTER(0x9cd0+i*4+0,stim_0[i]);
        WRITE_REGISTER(0x9cd0+i*4+1,stim_1[i]);
        WRITE_REGISTER(0x9cd0+i*4+2,stim_2[i]);
        WRITE_REGISTER(0x9cd0+i*4+3,stim_3[i]);

        // Set the mux signal for SCU2
        WRITE_REGISTER(0xdcd0+i*4+0,stim_0[i+4]);
        WRITE_REGISTER(0xdcd0+i*4+1,stim_1[i+4]);
        WRITE_REGISTER(0xdcd0+i*4+2,stim_2[i+4]);
        WRITE_REGISTER(0xdcd0+i*4+3,stim_3[i+4]);

        // Setting up SCU1
        WRITE_REGISTER(0x9ca0+2*i+0,0xffffffff);
        WRITE_REGISTER(0x9ca0+2*i+1,0xffffffff);

        // Setting up SCU2
        WRITE_REGISTER(0xdca0+2*i+0,0xffffffff);
        WRITE_REGISTER(0xdca0+2*i+1,0xffffffff);
    }
}
