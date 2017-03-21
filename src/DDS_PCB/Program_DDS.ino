/*Program the AD9833, see http://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf for more details
Calculates register values needed to program specific frequency and then uses AD9833_SendWord function to actually program the chip using SPI*/
#include "definitions.h"

void Set_AD9833_Frequency(long freq, int chan) {

	// Generating the frequency registers from the desired frequency
	// Frequency value to send to AD9833, needs to be separated into two parts, 14 bits long each
	unsigned long freq_word = (unsigned long)(0x10000000 / DDS_CLOCK_FREQUENCY * freq); 
	unsigned int msb = (freq_word >> 14); // 4 MSB of long data type aren't used, this gets the next 14 MSB
	unsigned int lsb = (freq_word & 0x3fff); // Gives the 14 LSB

	// Now we have two sets of 14 bits. Each word to be sent to the AD9833 should be 16 bits, with the 2 MSB indicating which register on the chip to send to
	// Here, using Register 0 (set 2 MSB to 01). To use Register 1, set 2MSB to 10 ( msb | 0x8000)
	msb = msb | 0x4000;
	lsb = lsb | 0x4000;

	// Set control register, frequency register (in two parts LSB and MSB) and phase register
	AD9833_SendWord(CONTROL_REGISTER_VALUE, chan);       // Control register
	AD9833_SendWord(lsb, chan);                         // Frequency Regsiter part 1 (LSB)
	AD9833_SendWord(msb, chan);                         // Frequency Register part 2 (MSB)
	AD9833_SendWord(PHASE_REGISTER_VALUE, chan);        // Phase regsister, don't need to change this at the moment, so set to 0 phase
}


void Set_AD9833_Phase(int phase, int chan) {
/* Function to set only the phase of a particular channel, frequency remains unchanged.
 Can be used to set particular phase difference between two channels */
 
	// Calculate phase word for required phase value
	// The phase on the DDS is equal to 12 LSB of phase word * 2*pi/PHASE_MAX
	unsigned int phase_word = (PHASE_REGISTER_VALUE + (phase / 360.0) * PHASE_MAX); 

	// Set Phase register
	AD9833_SendWord(phase_word, chan);        
}


void AD9833_SendWord(unsigned int data, int chan) {
	/*SPI Data write
	Can only send 8 bits at a time, so this splits up a 16 bit word and sends it as two parts
	Add in several ms delay before/after SPI transfer to account for propagation delay differences between digital isolators and switching circuitry */

	// Set FSYNC pin on the DDS chip we want to program
	Set_ADG984(chan);
	delay(DELAY_TIME);

	// Send 16 bit word as two 8 bit sections
	SPI.transfer((data >> 8) & 0xFF);
	SPI.transfer(data & 0xFF);

	// Disable SPI
	// We want to close all of the switches, so use some sentinel values >> than the number of switches (1000) to make this happen
	Set_ADG984(1000);
	delay(DELAY_TIME);
}


void Sweep_Freq (int freq_min, int freq_step, int freq_max, int chan, int delay_time) {
/* Sweeps the frequency output on a channel, with increment and max value set by user.
freq_min: starting frequency (Hz)
freq_step: increment (Hz)
freq_max: Stopping frequency (Hz)
chan: which channel to program
delay_time: how long to program each channel for (milliseconds) */
 
	for (int i = freq_min ; i <= freq_max; i = i + freq_step) {
		Set_AD9833_Frequency(i, chan);
		delay(delay_time);
	}
}


void Reset_DDS(int chan) {
// Reset single channel to midscale output (turns off AC output)
	AD9833_SendWord(RESET_CONTROL_REGISTER, chan);
}


void Reset_All(int n_chans) {
//Resets all channels to default output 
	for (int j = 1; j <= n_chans  ; j++) {
		Reset_DDS(j);
	}
}




