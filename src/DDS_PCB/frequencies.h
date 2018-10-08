/* User can set the frequencies for the DDS chips here */

#ifndef frequencies_h
#define frequencies_h

#define NUM_CHANNELS 6

#define FREQ_1 10000
#define FREQ_2 10000
#define FREQ_3 10000
#define FREQ_4 10000
#define FREQ_5 10000
#define FREQ_6 10000

#define PROGRAM_ALL 0 // Set to 1 to program all frequencies, set to 0 to only program one
#define CHANNEL_TO_PROGRAM 2


long Freqs[NUM_CHANNELS] = {  FREQ_1,
        FREQ_2,
        FREQ_3,
        FREQ_4,
        FREQ_5,
        FREQ_6};

#endif
