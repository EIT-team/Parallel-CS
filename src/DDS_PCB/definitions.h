#ifndef definitions_h
#define definitions_h

/*								*/
/* Main script definitions 		*/
/*								*/

// Arduino pin numberings
#define FSYNC_Pin 5
#define SDATA_SPI_Pin 11
#define SCLK_SPI_Pin  13

#define SCLK_SWITCH_Pin 9
#define SYNC_SWITCH_Pin 2
#define RESET_SWITCH_Pin 3
#define DIN_SWITCH_Pin 4

// Clock generator frequency, set by resistor R_SET on the PCB. This is used in the formula for setting the DDS sine wave frequency (Hz)
#define DDS_CLOCK_FREQUENCY 10e6

// Maximum value for injection when operating with limited length injections (milliseconds)
#define MAX_CAUTIOUS_INJECTION 25000

/*								*/
/* Program_DDS Defintions 		*/
/*								*/

#define DELAY_TIME 5    // Time to delay after programming switches
#define CONTROL_REGISTER_VALUE 0x2000    // Default value for control register of DDS chip
#define PHASE_REGISTER_VALUE 0xC000      // Default value for phase register of DDS chip
#define RESET_CONTROL_REGISTER 0x2100    // Reset control register of DDS to produce midscale output
#define PHASE_MAX 4096 // Maximum value of phase register


/*								*/
/* Program_Switch Definitions 	*/
/*								*/

#define N_SWITCHES 8

#endif
