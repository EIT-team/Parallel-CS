/* Code for programming the prototype Parallel CS board
  Tom Dowrick 19.10.2015
*/

// Pin Descriptions:
// SDATA_SPI - Data signal for SPI, used to program AD9833
// SCLK_SPI - Clock signal for SPI, used to program AD9833
//FSYNC - SYNC pin for SPI, used to program AD9833, but is routed by the ADG985 switch, where it is input to the DIN pin.

// Library for handling the SPI transfer
#include <SPI.h>

#include "definitions.h"
#include "frequencies.h"
#include <stdio.h>

void setup() {

  Serial.begin(SERIAL_BAUD_RATE);

  // For SPI data transfer setup
  pinMode(FSYNC_Pin, OUTPUT);
  pinMode(SDATA_SPI_Pin, OUTPUT);
  pinMode(SCLK_SPI_Pin, OUTPUT);

  digitalWrite(FSYNC_Pin, HIGH); //Set FSYNC High. FSYNC is active low
  digitalWrite(SDATA_SPI_Pin, LOW);
  digitalWrite(SCLK_SPI_Pin, LOW);

  // Arduino handles most of the underlying SPI transfer by itself, set it up here
  SPI.begin();  // Enable SPI
  SPI.setBitOrder(MSBFIRST);  // Send data Most significant bit first, this is necessary for the AD9833
  SPI.setDataMode(SPI_MODE2);  // Set SPI Mode 2 (Data captured on falling edge of clock, clock inversion on)
  SPI.setClockDivider(SPI_CLOCK_DIV128);    // Set clock divider (optional)
}

void loop() {

  byte byteRead; // Serial read data
  int val_to_program; // Frequency (Hz) or phase (degrees) value
  int chan_to_program;
  String what_to_program; // Freq, Phase, All or Reset

  // Allow setting of frequency/phase from serial monitor
  // Input 'freq val_to_program' to program frequency
  // 'phase val_to_program' to set phase

  while (1) {
    // Wait for Serial data to be received and parse it

    delay(ONE_SECOND_DELAY);
    Program_Freqs(Freqs, NUM_CHANNELS);
    
    if (Serial.available()) {

      what_to_program = Serial.readStringUntil(' '); // Get a string
      val_to_program = Serial.parseInt(); // Get an int
      chan_to_program = Serial.parseInt();

      // Process input and call appropriate commands
      // Can't use switch statement to do this as it won't accept a string as a comparator
      if (what_to_program == "reset") {
        Reset_DDS(chan_to_program);
      }

      else if (what_to_program == "freq")     {
        Set_AD9833_Frequency(val_to_program, chan_to_program);
      }

      else if (what_to_program == "phase")	 {
        Set_AD9833_Phase(val_to_program, chan_to_program);
      }

      else if (what_to_program == "switch")   {
        Program_ADG731(19, 20);
      }

      // default case - invalid command has been passed
      else {
        Serial.println("Command not recognised.");
      }
    }
  }
}


// Additional functions that may be helpful

void Program_Freqs (long Freqs [], int n_chans) {
  
  /* Loop through each freq/chan pair and program the switches/DDS chip */

  Serial.println("Programming default frequencies");
  for (int i = 0; i < n_chans; i++) {
    Serial.print("Programming channel ");
    Serial.println(i);
    Set_AD9833_Frequency(Freqs[i], i + 1);
  }
}

void Program_Then_Turn_Off(long freq, unsigned int on_time_milli, int chan) {

  /*Program the DDS chip for a given time, then turn off */

  //Program all of the channels, wait a specified time, then turn off.
  Set_AD9833_Frequency(freq, chan);
  delay(on_time_milli);
  Reset_DDS(chan);


}

