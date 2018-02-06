/* User can set the frequencies for the DDS chips here */

#ifndef frequencies_h
#define frequencies_h

#define NUM_CHANNELS 6

#define FREQ_1 1000
#define FREQ_2 2000
#define FREQ_3 7000
#define FREQ_4 9000
#define FREQ_5 10000
#define FREQ_6 11000

long Freqs[NUM_CHANNELS] = {  FREQ_1,
        FREQ_2,
        FREQ_3,
        FREQ_4,
        FREQ_5,
        FREQ_6};

#endif
