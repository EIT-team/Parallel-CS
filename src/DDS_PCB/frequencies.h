/* User can set the frequencies for the DDS chips here */

#ifndef frequencies_h
#define frequencies_h

#define NUM_CHANNELS 8

#define FREQ_1 1000
#define FREQ_2 2000
#define FREQ_3 3000
#define FREQ_4 4000
#define FREQ_5 5000
#define FREQ_6 6000
#define FREQ_7 7000
#define FREQ_8 8000

long Freqs[NUM_CHANNELS] = {	FREQ_1,
								FREQ_2,
								FREQ_3,
								FREQ_4,
								FREQ_5,
								FREQ_6,
								FREQ_7,
								FREQ_8};

#endif
