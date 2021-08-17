#include "stim_pattern.h"
#include "led.h"

#include "pattern_full.h"
#include "pattern_circle.h"

int circuit_type[8];                 // Encode for each HS, what type it is.
int num_stim;                        // Number of stimuli used in a pattern
int num_circuits[8];                 // Number of circuits per HS
int num_electrodes[8];               // Number of electrodes per circuit
int random_pattern;                  // if not zero, choose patterns at random
long long int period_us;             // Number of us between each pattern
int inter_spike_period;              // Number of us between two stimuli in a pattern
int multi_pattern;                   // If not zero, stimulate with two patterns at a time

struct Pattern stim_patterns[8][MAX_NUMBER_CIRCUITS];
Uint32 counter_stimulation;

void stim_pattern_init()
{
    int h,i,j,k;
    for (i=0;i<8;i++)
    {
        num_circuits[i]   = 1;                // Number of circuits per HS
        num_electrodes[i] = 60;               // Number of electrodes per circuit
    }
    for(h=0;h<8;h++)
    {
        for(i=0;i<MAX_NUMBER_CIRCUITS;i++)
        {
            for(j=0;j<MAX_NUMBER_PROBABILITIES;j++)
                stim_patterns[h][i].probabilites[j]    = j % MAX_NUMBER_PATTERNS;
            for(j=0;j<MAX_NUMBER_PATTERNS;j++)
                for(k=0;k<MAX_NUMBER_STIMULATIONS;k++)
                    stim_patterns[h][i].patterns[j][k] = 0;
            stim_patterns[h][i].current_pattern        = 0;
            for(j=0;j<60;j++)
                stim_patterns[h][i].electrodes[j] = j;
        }
        set_network_type(h, circuit_type[h]); // init every HS to the corresponding circuit type
    }
    counter_stimulation = 0;
}

void set_network_type(int HS, int type)
{
    // 0: No mask
    // 1: 5x3 0 circles

    circuit_type[HS] = type;
    switch(type)
    {
        case 0:
            num_circuits[HS]   = 1;
            num_electrodes[HS] = 60;
            pattern_full_init(HS);
            break;
        case 1:
            num_circuits[HS]   = 15;
            num_electrodes[HS] = 4;
            pattern_circle_init(HS);
            break;

        default:
            {};
    }
}

void get_single_stimulation_pattern(int HS, int circuit, int position, int *elec_stim_left, int *elec_stim_right)
{
    int pattern_index, electrode;
    pattern_index = stim_patterns[HS][circuit].current_pattern;
    if(position == 0)
    {
        if(random_pattern)
            pattern_index = rand() % MAX_NUMBER_PROBABILITIES;
        else
            pattern_index = (pattern_index + 1) % MAX_NUMBER_PROBABILITIES;
        stim_patterns[HS][circuit].current_pattern = pattern_index;

        // Write the stim patterns into the mailbox. Done so for the last stimulation pattern on the last HS
        if((HS == 7) && (circuit == (num_circuits[7]-1)))
        {
            int h,i;

            // In total, this takes 0x80 bytes data + 0x04 bytes in the mailbox, which equals 33 words (32 bit). We would need a total of 0x108 addresses to save whole slot
            // + 2 times the counter. Unless this becomes a problem again, I skip the last 8 circuits on HS 8, since I do not need them. ToDo (maybe): Better mem allocation
            WRITE_REGISTER(0x1E00+0x0004*(8*MAX_NUMBER_CIRCUITS/4+1),READ_REGISTER(0x1E00));
            WRITE_REGISTER(0x1E00,counter_stimulation);
            for(h=0;h<8;h++)
            {
                for(i=0;i<MAX_NUMBER_CIRCUITS/4;i++)
                {
                    int pos = h*MAX_NUMBER_CIRCUITS/4 + i + 1;       // +1 for the counter_stimulation
                    if(0x0004*(pos+8*MAX_NUMBER_CIRCUITS/4) < 0x100) // This will be true except for the last 8 circuits of HS 8. These are not backuped. Only relevant, if there are more than 8 circuits on this HS.
                        WRITE_REGISTER(0x1E00+0x0004*(pos+8*MAX_NUMBER_CIRCUITS/4+1),READ_REGISTER(0x1E00+0x0004*pos));
                    WRITE_REGISTER(0x1E00+0x0004*pos,
                    // This part sends the pattern ID.
                                   ((stim_patterns[h][i*4+0].probabilites[stim_patterns[h][i*4+0].current_pattern] & 0xff) <<  0) +
                                   ((stim_patterns[h][i*4+1].probabilites[stim_patterns[h][i*4+1].current_pattern] & 0xff) <<  8) +
                                   ((stim_patterns[h][i*4+2].probabilites[stim_patterns[h][i*4+2].current_pattern] & 0xff) << 16) +
                                   ((stim_patterns[h][i*4+3].probabilites[stim_patterns[h][i*4+3].current_pattern] & 0xff) << 24));

                    // ---------------------------------------------------------------------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------------------------------------------------------------------
                    //
                    //
                    // BUG: Fixed 201015 16:05. Consequence of bug: The wrong network pattern ids were sent to the computer. Stimulation was still correct. See labjournal 3.74.
                    //
                    //
                    // ---------------------------------------------------------------------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------------------------------------------------------------------

                    // This part sends the probability ID.
                    //             ((stim_patterns[h][i*4+0].current_pattern & 0xff) <<  0) +
                    //             ((stim_patterns[h][i*4+1].current_pattern & 0xff) <<  8) +
                    //             ((stim_patterns[h][i*4+2].current_pattern & 0xff) << 16) +
                    //             ((stim_patterns[h][i*4+3].current_pattern & 0xff) << 24));
                }
            }
        }
    }
    electrode = (int)stim_patterns[HS][circuit].patterns[stim_patterns[HS][circuit].probabilites[pattern_index]][position];

    if(electrode == 0)           // Do not stimulate any electrode
    {}
    else if(electrode > num_electrodes[HS]) // This should not happen
    {}
    else
    {
        electrode = stim_patterns[HS][circuit].electrodes[electrode-1];
        if(electrode>=30)
            *elec_stim_right = *elec_stim_right | (1 << (electrode-30));
        else
            *elec_stim_left  = *elec_stim_left  | (1 <<  electrode);

        if (multi_pattern == true) // This is only true if two patterns should be stimulated at the same time
        {
            electrode = (int)stim_patterns[HS][circuit].patterns[1+stim_patterns[HS][circuit].probabilites[pattern_index]][position];

            if(electrode == 0)           // Do not stimulate any electrode
            {}
            else if(electrode > num_electrodes[HS]) // This should not happen
            {}
            else
            {
                electrode = stim_patterns[HS][circuit].electrodes[electrode-1];
                if(electrode>=30)
                    *elec_stim_right = *elec_stim_right | (1 << (electrode-30));
                else
                    *elec_stim_left  = *elec_stim_left  | (1 <<  electrode);
            }
        }
    }
}

void get_stimulation_pattern(int HS, int position, int *elec_stim_left, int *elec_stim_right)
{
    int i;

    if(position == 0 && HS == 0)
        counter_stimulation += 1;

    *elec_stim_left  = 0;
    *elec_stim_right = 0;

    for(i=0;i<num_circuits[HS];i++)
    {
        get_single_stimulation_pattern(HS,i, position, elec_stim_left, elec_stim_right);
        /*if((i>=8) && (i<16) && (HS==0) && (position==2))
        {
            WRITE_REGISTER(0x1600+4*(i-8),*elec_stim_left);
            WRITE_REGISTER(0x1600+4*(i-8),*elec_stim_right);
        }*/
    }
    WRITE_REGISTER(0x1600+4*7,multi_pattern);
}

void stimulation_pattern_lower_update(int HS, int circuit)
{
    // This functions sets the spikes 0-7. Note that 0 is the first stim and num_stim-1 is the last stim.
    int i,j;
    for(i=0;i<256;i++)
    {
        Uint32 data_package = READ_REGISTER(0x1A00+0x0004*i);
        int size            = (256/MAX_NUMBER_PATTERNS); //2 if MAX_NUMBER_PATTERNS == 128
        int pattern         = i/size;
        int offset          = i%size;
        for(j=0;j<4;j++)
        {
            stim_patterns[HS][circuit].patterns[pattern][j+4*offset] = 0xff & (data_package >> (j*8));
        }
    }
    /*
    WRITE_REGISTER(0x1600+4*0,stim_patterns[0][14].patterns[0][0]);
    WRITE_REGISTER(0x1600+4*1,stim_patterns[0][14].patterns[0][1]);
    WRITE_REGISTER(0x1600+4*2,stim_patterns[0][14].patterns[0][2]);
    WRITE_REGISTER(0x1600+4*3,stim_patterns[0][14].patterns[0][3]);
    WRITE_REGISTER(0x1600+4*4,stim_patterns[0][14].patterns[0][4]);
    WRITE_REGISTER(0x1600+4*5,stim_patterns[0][14].patterns[0][5]);
    WRITE_REGISTER(0x1600+4*6,stim_patterns[0][14].patterns[0][6]);
    WRITE_REGISTER(0x1600+4*7,stim_patterns[0][14].patterns[0][7]);
    */
}

void stimulation_pattern_upper_update(int HS, int circuit)
{
    int i,j;
    for(i=0;i<256;i++)
    {
        Uint32 data_package = READ_REGISTER(0x1A00+0x0004*i);
        int size            = (256/MAX_NUMBER_PATTERNS);
        int pattern         = i/size;
        int offset          = i%size + size;
        for(j=0;j<4;j++)
        {
            stim_patterns[HS][circuit].patterns[pattern][j+4*offset] = 0xff & (data_package >> (j*8));
        }
    }
}

void stimulation_probability_update(int HS, int circuit)
{
    int i;
    for(i=0;i<256;i++)
    {
        stim_patterns[HS][circuit].probabilites[i] = READ_REGISTER(0x1A00+0x0004*i) & 0x007F;
    }
}
