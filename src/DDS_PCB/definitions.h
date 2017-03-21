#ifndef definitions_h
#define definitions_h

/* Main script definitions 		*/

// Arduino pin numberings
// Pin Descriptions:
// SDATA_SPI - Data signal for SPI, used to program AD9833
// SCLK_SPI - Clock signal for SPI, used to program AD9833
// FSYNC - SYNC pin for SPI, used to program AD9833, but is routed by the ADG985 switch, where it is input to the DIN pin.
// SCLK_Switch - Clock for ADG984 switch
// SYNC_Switch - Sync pin for switch. Active low, set low allows progamming of switches
// RESET_Switch - Reset pin for switch. Active low, Resets all switches to default, open, position.
// DIN_Switch - Send the values to open/close switches

#define FSYNC_Pin 5
#define SDATA_SPI_Pin 11
#define SCLK_SPI_Pin  13

#define SCLK_SWITCH_Pin 9
#define SYNC_SWITCH_Pin 2
#define RESET_SWITCH_Pin 3
#define DIN_SWITCH_Pin 4

// Maximum value for injection when operating with limited length injections (milliseconds)
#define MAX_CAUTIOUS_INJECTION 25000

#define ONE_SECOND_DELAY 1000
#define SERIAL_BAUD_RATE 9600

#define PRINT_BUFFER_SIZE 50


/* Program_DDS Defintions 		*/

// Clock generator frequency, set by resistor R_SET on the PCB. This is used in the formula for setting the DDS sine wave frequency (Hz)
#define DDS_CLOCK_FREQUENCY 10e6

#define SWITCH_DELAY_TIME 5    // Time to delay after programming switches
#define CONTROL_REGISTER_VALUE 0x2000    // Default value for control register of DDS chip
#define PHASE_REGISTER_VALUE 0xC000      // Default value for phase register of DDS chip
#define RESET_CONTROL_REGISTER 0x2100    // Reset control register of DDS to produce midscale output
#define PHASE_REGISTER_MAX 4096 // Maximum value of phase register
#define CLOSE_ALL_SWITCHES 1000 // If a value > N_SWITCHES is passed, then all switches are closed
#define DDS_PROGRAM_CONSTANT 0x10000000 // 2^28
#define MAX_FREQUENCY 1e5

// Bit masks and shifts
#define LSB_BIT_MASK 0x3fff // Gives the 14 LSB of an unsigned long
#define SPI_BIT_MASK 0xFF // Gives 8 MSB of an unsigned int
#define FREQ_REG_MASK 0x4000 	//Use Register 0 (set 2 MSB to 01). To use Register 1, set 2MSB to 10 (0x8000)


/* Program_Switch Definitions 	*/

#define N_SWITCHES 8

#endif
