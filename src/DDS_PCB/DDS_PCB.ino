// Library for handling the SPI transfer
#include <SPI.h>

#include "definitions.h"
#include "frequencies.h"
#include <stdio.h>

void setup() {

  Serial.begin(SERIAL_BAUD_RATE);

  pinMode(FSYNC1_Pin, OUTPUT);
  pinMode(FSYNC2_Pin, OUTPUT);
  pinMode(FSYNC3_Pin, OUTPUT);
  pinMode(FSYNC4_Pin, OUTPUT);
  pinMode(FSYNC5_Pin, OUTPUT);
  pinMode(FSYNC6_Pin, OUTPUT);

    //Set FSYNC pins High. FSYNC is active low
  digitalWrite(FSYNC1_Pin, HIGH); 
  digitalWrite(FSYNC2_Pin, HIGH); 
  digitalWrite(FSYNC3_Pin, HIGH);
  digitalWrite(FSYNC4_Pin, HIGH); 
  digitalWrite(FSYNC5_Pin, HIGH); 
  digitalWrite(FSYNC6_Pin, HIGH); 

  // For SPI data transfer setup
  pinMode(SDATA_SPI_Pin, OUTPUT);
  pinMode(SCLK_SPI_Pin, OUTPUT);

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
  int val_to_program; // Frequency (Hz), phase (degrees), time etc.
  int chan_to_program;
  String what_to_program; //  Command to execute

  // Allow setting of frequency/phase from serial monitor
  // Input 'freq val_to_program' to program frequency
  // 'phase val_to_program' to set phase
    delay(ONE_SECOND_DELAY);
    if (PROGRAM_ALL) {
      Program_Freqs(Freqs, NUM_CHANNELS);
    }

    else {
      Program_One_Frequency(Freqs, CHANNEL_TO_PROGRAM);  
    }
    
  while (1) {
    // Wait for Serial data to be received and parse it


    
    if (Serial.available()) {

      what_to_program = Serial.readStringUntil(' '); // Get a string
      val_to_program = Serial.parseInt(); // Get an int
      chan_to_program = Serial.parseInt();

      // Process input and call appropriate commands
      // Can't use switch statement to do this as it won't accept a string as a comparator
      if (what_to_program == "reset")  {
        Reset_DDS(chan_to_program);
      }

      else if (what_to_program == "freq")     {
        Set_AD9833_Frequency(val_to_program, chan_to_program);
      }

      else if (what_to_program == "phase")	 {
        Set_AD9833_Phase(val_to_program, chan_to_program);
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

void Program_One_Frequency(long Freqs [], int chan_to_turn_on) {
  /* Only program one current source, set all the rest to off/midscale DC output */

  Serial.print("Programming only source ");
  Serial.println(chan_to_turn_on);

  Reset_All();
  delay(100);
  Set_AD9833_Frequency(Freqs[chan_to_turn_on], chan_to_turn_on);    

}

void Sweep_Freq (int freq_min, int freq_step, int freq_max, int on_time, int chan) {
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
  /* Reset channel to midscale output (turns off AC output) */
  AD9833_SendWord(RESET_CONTROL_REGISTER, chan);
}


void Reset_All() {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    Reset_DDS(i);
  }
 
}

