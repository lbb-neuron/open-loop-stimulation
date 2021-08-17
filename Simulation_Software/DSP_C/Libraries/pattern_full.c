#include "pattern_full.h"
#include "stim_pattern.h"

void pattern_full_init(int HS)
{
    Uint8 circle_pattern_electrodes[1][60] = {{0,1,2,3,4,5,6,7,8,9,
                                               10,11,12,13,14,15,16,17,18,19,
                                               20,21,22,23,24,25,26,27,28,29,
                                               30,31,32,33,34,35,36,37,38,39,
                                               40,41,42,43,44,45,46,47,48,49,
                                               50,51,52,53,54,55,56,57,58,59}};

    int i,j,k;
    num_circuits[HS]   = 1;                // Number of circuits per HS
    num_electrodes[HS] = 60;               // Number of electrodes per circuit
    for(i=0;i<num_circuits[HS];i++)
    {
        if(1==0) // This creates all patterns identical with first n electrodes being stimulated
        {
            for(j=0;j<MAX_NUMBER_PROBABILITIES;j++)
                stim_patterns[HS][i].probabilites[j]    = j % MAX_NUMBER_PATTERNS;
            for(j=0;j<MAX_NUMBER_PATTERNS;j++)
                for(k=0;k<num_stim;k++)
                    stim_patterns[HS][i].patterns[j][k] = (1+k)%num_electrodes[HS];
            stim_patterns[HS][i].current_pattern        = 0;
            for(j=0;j<num_electrodes[HS];j++)
                stim_patterns[HS][i].electrodes[j] = circle_pattern_electrodes[i][j];
        }
        else    // Here, each pattern is different. Stimulate the (i%60)^th electrode for the i^th pattern
        {
            for(j=0;j<MAX_NUMBER_PROBABILITIES;j++)
                stim_patterns[HS][i].probabilites[j]    = j % MAX_NUMBER_PATTERNS;
            for(j=0;j<MAX_NUMBER_PATTERNS;j++)
                for(k=0;k<num_stim;k++)
                {
                    if(k == num_stim-1)
                        if(HS==0)
                            stim_patterns[HS][i].patterns[j][k] = j%60+1;
                        else if(HS==1)
                            stim_patterns[HS][i].patterns[j][k] = (128-j-1)%60+1;
                        else if(HS==2)
                            stim_patterns[HS][i].patterns[j][k] = ((j/6) + (j%6)*10)%60+1;
                        else if(HS==3)
                            stim_patterns[HS][i].patterns[j][k] = (128-((j/6) + ((j)%6)*10))%60;
                        else
                            stim_patterns[HS][i].patterns[j][k] = 0;
                    else
                        stim_patterns[HS][i].patterns[j][k] = 0;
                }
            stim_patterns[HS][i].current_pattern        = 0;
            for(j=0;j<num_electrodes[HS];j++)
                stim_patterns[HS][i].electrodes[j] = circle_pattern_electrodes[i][j];
        }
    }
}
