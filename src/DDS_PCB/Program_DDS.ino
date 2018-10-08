/* Functions to Program the AD9833, see http://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf for more details on the algorithm and technical details.

For a required frequency/phase value, a frequency/phase word is calculated which must be sent to the DDS chip over SPI */

#include "definitions.h"
#include "Arduino.h"
#include <stdio.h>

void AD9833_SendWord(unsigned int data, int chan) {
	/* SPI Data write can only send 8 bits at a time, so this splits up a 16 bit word and sends it as two parts. Unsigned int is 32 bits long, only the 16MSB are used.
	A few ms of delay are added before/after SPI transfer to account for propagation delay differences between digital isolators and switching circuitry */

	// Set FSYNC pin on the DDS chip we want to program
  Set_FSYNC_LOW(chan);
	delay(SWITCH_DELAY_TIME);

	// SPI.transfer only sends 8 bits a a time
	// Send 16 bit word as two 8 bit sections
	SPI.transfer( (data >> 8) & SPI_BIT_MASK);
	SPI.transfer(data & SPI_BIT_MASK);

	// Disable SPI
	// We want to close all of the switches, so use some sentinel values >> than the number of switches (e.g. 1000) to make this happen
  Set_FSYNC_HIGH(chan);

	delay(SWITCH_DELAY_TIME);
}

unsigned long Get_Frequency_Word(long freq) {
	// Calculate the frequency word, which is 28 bits long.
	unsigned long freq_word = (unsigned long)(DDS_PROGRAM_CONSTANT / DDS_CLOCK_FREQUENCY * freq);
	return freq_word;
}


unsigned int Get_MSB(unsigned long freq_word) {
	// Get the MSB of the frequency word
	// The 4 MSB of the frequency word aren't used, this gets the next 14 MSB
	unsigned int msb = freq_word >> 14;
	
	// Set the two MSB to indiciate which DDS register data will be sent to.
	return msb | FREQ_REG_MASK;;
}


unsigned int Get_LSB(unsigned long freq_word) {
	// Get the LSB of the frequency word
	unsigned int lsb = freq_word & LSB_BIT_MASK;  // Gives the 14 LSB
	
	// Set the two MSB to indiciate which DDS register data will be sent to.
	return lsb | FREQ_REG_MASK;;
}


int Set_AD9833_Frequency(long freq, int chan) {
	/* Generate the frequency word, msb and lsb, and send to the DDS chips */
	
	// Abort if frequency is 0, negative or greater than MAX_FREQUENCY
	if (freq <= 0 || freq > MAX_FREQUENCY) {
		Serial.println("Invalid Frequency.");
		return -1;
	}
	
	// Generate the frequency register values for the desired frequency
	// Frequency value to send to AD9833, needs to be separated into two parts, 14 bits long each
	unsigned long freq_word =  Get_Frequency_Word(freq);
	unsigned int msb = Get_MSB(freq_word); 
	unsigned int lsb = Get_LSB(freq_word);

	// Set control register, frequency register (in two parts LSB and MSB) and phase register
	AD9833_SendWord(CONTROL_REGISTER_VALUE, chan);       // Control register
	AD9833_SendWord(lsb, chan);                         // Frequency Regsiter part 1 (LSB)
	AD9833_SendWord(msb, chan);                         // Frequency Register part 2 (MSB)
	AD9833_SendWord(PHASE_REGISTER_VALUE, chan);        // Phase regsister, don't need to change this at the moment, so set to 0 phase
	
	// Everything OK, print confirmation
	char buffer[PRINT_BUFFER_SIZE];
	snprintf(buffer, PRINT_BUFFER_SIZE, "Channel programmed with frequency %d", freq);
	Serial.println(buffer);
	return 1;
}


unsigned int Set_AD9833_Phase(int phase,int chan) {
	/* Function to set only the phase of a particular channel, frequency remains unchanged.
Can be used to set particular phase difference between two channels */
	
	// Check phase is between 0-360
	if (phase < 0 || phase > 360) {
		Serial.println("Phase out of valid range (0-360)");
		phase = 0;
	}
	
	// Calculate phase word for required phase value
	// The phase on the DDS is equal to 12 LSB of phase word * 2*pi/PHASE_MAX
	unsigned int phase_word = (PHASE_REGISTER_VALUE + (phase / 360.0) * PHASE_REGISTER_MAX); 

	// Set Phase register
	AD9833_SendWord(phase_word, chan);        
	
	// Print confirmation
	char buffer[PRINT_BUFFER_SIZE];
	snprintf(buffer, PRINT_BUFFER_SIZE, "Channel programmed with phase %d", phase);
	Serial.println(buffer);
	
	return phase_word;
}


void Set_FSYNC_LOW(int chan) {
  // Set the FSYNC pin for a partiular channel low
  
  switch (chan) {
    case 1:
        digitalWrite(FSYNC1_Pin,LOW);
        break;
        
    case 2:
        digitalWrite(FSYNC2_Pin,LOW);
        break;
        
    case 3:
        digitalWrite(FSYNC3_Pin,LOW);
        break;
        
    case 4:
        digitalWrite(FSYNC4_Pin,LOW);
        break;
        
    case 5:
        digitalWrite(FSYNC5_Pin,LOW);
        break;
        
    case 6:
        digitalWrite(FSYNC6_Pin,LOW);
        break;
  }
}


void Set_FSYNC_HIGH(int chan) {
  // Set the FSYNC pin for a partiular channel high

  switch (chan) {
    case 1:
        digitalWrite(FSYNC1_Pin,HIGH);
        break;
        
    case 2:
        digitalWrite(FSYNC2_Pin,HIGH);
        break;
        
    case 3:
        digitalWrite(FSYNC3_Pin,HIGH);
        break;
        
    case 4:
        digitalWrite(FSYNC4_Pin,HIGH);
        break;
        
    case 5:
        digitalWrite(FSYNC5_Pin,HIGH);
        break;
        
    case 6:
        digitalWrite(FSYNC6_Pin,HIGH);
        break;
  }
}


