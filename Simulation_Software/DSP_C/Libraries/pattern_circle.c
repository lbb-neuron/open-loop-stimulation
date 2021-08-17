#include "pattern_circle.h"
#include "stim_pattern.h"

void pattern_circle_init(int HS)
{
    Uint8 circle_pattern_electrodes[15][4] = {{1,11,10,0},
                                             {21,31,30,20},
                                             {41,51,50,40},
                                             {3,13,12,2},
                                             {23,33,32,22},
                                             {43,53,52,42},
                                             {5,15,14,4},
                                             {25,35,34,24},
                                             {45,55,54,44},
                                             {7,17,16,6},
                                             {27,37,36,26},
                                             {47,57,56,46},
                                             {9,19,18,8},
                                             {29,39,38,28},
                                             {49,59,58,48}};
    int i,j,k;
    num_circuits[HS]   = 15;          // Number of circuits per HS
    num_electrodes[HS] = 4;           // Number of electrodes per circuit
    for(i=0;i<num_circuits[HS];i++)
    {
        for(j=0;j<MAX_NUMBER_PROBABILITIES;j++)
            stim_patterns[HS][i].probabilites[j]    = j % MAX_NUMBER_PATTERNS;
        for(j=0;j<MAX_NUMBER_PATTERNS;j++)
            for(k=0;k<num_stim;k++)
            {
                if(k+HS < 5)
                    stim_patterns[HS][i].patterns[j][k] = 2-(((j%(1<<num_stim))>>(num_stim-k-1))&1);//(1+k)%num_electrodes[HS];
                else
                    stim_patterns[HS][i].patterns[j][k] = 0;
            }
        stim_patterns[HS][i].current_pattern        = 0;
        for(j=0;j<num_electrodes[HS];j++)
            stim_patterns[HS][i].electrodes[j] = circle_pattern_electrodes[i][j];
    }
}
