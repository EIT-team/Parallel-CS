/* Functions to Program the AD9833, see http://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf for more details on the algorithm and technical details.

For a required frequency/phase value, a frequency/phase word is calculated which must be sent to the DDS chip over SPI */

#include "definitions.h"
#include "Arduino.h"

void AD9833_SendWord(unsigned int data, int chan) {
	/* SPI Data write can only send 8 bits at a time, so this splits up a 16 bit word and sends it as two parts. Unsigned int is 32 bits long, only the 16MSB are used.
	A few ms of delay are added before/after SPI transfer to account for propagation delay differences between digital isolators and switching circuitry */

	// Set FSYNC pin on the DDS chip we want to program
	Set_ADG984(chan);
	delay(SWITCH_DELAY_TIME);

	// SPI.transfer only sends 8 bits a a time
	// Send 16 bit word as two 8 bit sections
	SPI.transfer( (data >> 8) & SPI_BIT_MASK);
	SPI.transfer(data & SPI_BIT_MASK);

	// Disable SPI
	// We want to close all of the switches, so use some sentinel values >> than the number of switches (e.g. 1000) to make this happen
	Set_ADG984(CLOSE_ALL_SWITCHES);
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
	
	// Abort if frequency is 0, negative or greater than 100kHz
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
	
	// Everything OK
	return 1;
}


unsigned int Set_AD9833_Phase(int phase, int chan) {
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
	
	return phase_word;
}



void Sweep_Freq (int freq_min, int freq_step, int freq_max, int chan, int on_time) {
/* Sweeps the frequency output on a channel, with increment and max value set by user.
freq_min: starting frequency (Hz)
freq_step: increment (Hz)
freq_max: Stopping frequency (Hz)
chan: which channel to program
on_time: how long to program each channel for (milliseconds) */
 
	for (int i = freq_min ; i <= freq_max; i = i + freq_step) {
		Set_AD9833_Frequency(i, chan);
		delay(on_time);
	}
}


void Reset_DDS(int chan) {
/* Reset single channel to midscale output (turns off AC output) */
	AD9833_SendWord(RESET_CONTROL_REGISTER, chan);
}


void Reset_All(int n_chans) {
/* Resets all channels to default output */
	for (int j = 1; j <= n_chans  ; j++) {
		Reset_DDS(j);
	}
}




