#include "definitions.h"

void Set_ADG984 (int channel_to_open) {
/* Opens a single switch on the ADG984, to allow FSYNC to be routed to the appropriate DDS IC 

19.10.15 Using an unconventional way to set the FSYNC pin on the DDS chip. Because the output from the digital isolators is low by default, and FSYNC is an active low signal, we actually want them all to be high.
Achieve this by sending an FSYNC signal from the microcontroller which is always high, then a paricular DDS chip's pin high, by opening the corresponding switch on the ADG984, closing the switch will set it low.

Inputs  - channel_to_open, this is the channel we want to activate, i.e. set to digital LOW */

	//Program the switches
	//Set SYNC low, clock through switch setting values (0 or 1) , then set SYNC high to actually open/close switches
	digitalWrite(SYNC_SWITCH_Pin, LOW);

	//Iterate once for each switch.
 // Do this in descending order as that it how they are clocked into the switch IC
	for (int j = N_SWITCHES; j > 0; j--) {

		//Sets the target channel to digital LOW, by open circuting the switch, causing the digital isolator to take its default value of digital LOW
		if (j == channel_to_open) {
			digitalWrite(DIN_SWITCH_Pin, LOW);
		}
		//Sets all other channels digital HIGH, by connecting to the high FYSNC signal coming from the microcontroller.
		else {
			digitalWrite(DIN_SWITCH_Pin, HIGH);
		}
		
		//Generate clock pulse to clock in switch values to DIN pins.
		digitalWrite(SCLK_SWITCH_Pin, HIGH);
		digitalWrite(SCLK_SWITCH_Pin, LOW);

		//Set DIN low again
		digitalWrite(DIN_SWITCH_Pin, LOW);
	}

	//Set SYNC high to update switches
	digitalWrite(SYNC_SWITCH_Pin, HIGH);
}

