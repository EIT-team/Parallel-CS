#ifndef definitions_h
#define definitions_h

// Arduino pin numberings
#define SDATA_SPI_Pin 11  // SDATA_SPI - Data signal for SPI, used to program AD9833
#define SCLK_SPI_Pin  13  // SCLK_SPI - Clock signal for SPI, used to program AD9833

// Parallel CS FSYNC Pins for Pro Mini
#define FSYNC1_Pin 2
#define FSYNC2_Pin 3
#define FSYNC3_Pin 4
#define FSYNC4_Pin 19
#define FSYNC5_Pin 20
#define FSYNC6_Pin 21

// Clock generator frequency, set by resistor R_SET on the PCB. This is used in the formula for setting the DDS sine wave frequency (Hz)
#define DDS_CLOCK_FREQUENCY 10e6

#define ONE_SECOND_DELAY 1000 //1000 milliseconds
#define SERIAL_BAUD_RATE 9600

#define PRINT_BUFFER_SIZE 50

#define SWITCH_DELAY_TIME 2    // Time to delay after programming switches
#define CONTROL_REGISTER_VALUE 0x2000    // Default value for control register of DDS chip
#define PHASE_REGISTER_VALUE 0xC000      // Default value for phase register of DDS chip
#define RESET_CONTROL_REGISTER 0x2100    // Reset control register of DDS to produce midscale output
#define PHASE_REGISTER_MAX 4096 // Maximum value of phase register
#define DDS_PROGRAM_CONSTANT 0x10000000 // 2^28
#define MAX_FREQUENCY 1e5 //Maximum frequency value, used for sanity check.

// Maximum value for injection when operating with limited length injections (milliseconds)
#define MAX_CAUTIOUS_INJECTION 25000

// Bit masks and shifts
#define LSB_BIT_MASK 0x3fff // Gives the 14 LSB of an unsigned long
#define SPI_BIT_MASK 0xFF // Gives 8 MSB of an unsigned int
#define FREQ_REG_MASK 0x4000 	//Use Register 0 (set 2 MSB to 01). To use Register 1, set 2MSB to 10 (0x8000)

#endif
