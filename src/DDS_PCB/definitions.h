#ifndef definitions_h
#define definitions_h

/*								*/
/* Main script definitions 		*/
/*								*/

// Arduino pin numberings
#define FSYNC_Pin 5
#define SDATA_SPI_Pin 11
#define SCLK_SPI_Pin  13

// ADG731 Sync pins
#define ADG731_SYNC1 3
#define ADG731_SYNC2 2
#define NUM_SWITCH_CHANNELS 32
#define SCLK_SWITCH_Pin 9
#define SYNC_SWITCH_Pin 2
#define RESET_SWITCH_Pin 3
#define DIN_SWITCH_Pin 4

// Clock generator frequency, set by resistor R_SET on the PCB. This is used in the formula for setting the DDS sine wave frequency (Hz)
#define DDS_CLOCK_FREQUENCY 10e6

// Maximum value for injection when operating with limited length injections (milliseconds)
#define MAX_CAUTIOUS_INJECTION 25000

#define ONE_SECOND_DELAY 1000
#define SERIAL_BAUD_RATE 9600

#define PRINT_BUFFER_SIZE 50
/*								*/
/* Program_DDS Defintions 		*/
/*								*/

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

/*								*/
/* Program_Switch Definitions 	*/
/*								*/

#define N_SWITCHES 8

// There isn't a direct mapping from the switch channels to the elctrode numbering
// Use these maps to address the correct electrode
int electrode_map_switch[2][32] = { {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 31, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 },
                                    {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 21, 22, 23, 24, 25, 26, 13, 14, 15, 16, 32, 31, 30, 29, 20, 19, 18, 17, 27, 28} };

#endif
