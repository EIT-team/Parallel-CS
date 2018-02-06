#ifndef definitions_h
#define definitions_h

/*								*/
/* Main script definitions 		*/
/*								*/

// Arduino pin numberings
#define FSYNC_Pin 5
#define SDATA_SPI_Pin 11
#define SCLK_SPI_Pin  13

#define SYNC1_Pin 3
#define SYNC2_Pin 2

// Parallel CS FSYNC Pins for Pro Mini
#define FSYNC1_Pin 2
#define FSYNC2_Pin 3
#define FSYNC3_Pin 4
#define FSYNC4_Pin 15
#define FSYNC5_Pin 16
#define FSYNC6_Pin 17

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

#define SWITCH_DELAY_TIME 2    // Time to delay after programming switches
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


/*
 * Switches definitions
 * 
  */
#define ADG731_ENABLE_MASK 0x00000000 // Sets EN and CSA bits low

#endif
