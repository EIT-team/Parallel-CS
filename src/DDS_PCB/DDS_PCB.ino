/* Code for programming the prototype Parallel CS board
  Tom Dowrick 19.10.2015
*/

// Pin Descriptions:
// SDATA_SPI - Data signal for SPI, used to program AD9833
// SCLK_SPI - Clock signal for SPI, used to program AD9833
//FSYNC - SYNC pin for SPI, used to program AD9833, but is routed by the ADG985 switch, where it is input to the DIN pin.
//  SCLK_Switch - Clock for ADG984 switch
// SYNC_Switch - Sync pin for switch. Active low, set low allows progamming of switches
//  RESET_Switch - Reset pin for switch. Active low, Resets all switches to default, open, position.
// DIN_Switch - Send the values to open/close switches

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

  //For Digital Switch
  pinMode(SCLK_SWITCH_Pin, OUTPUT);
  pinMode(SYNC_SWITCH_Pin, OUTPUT);
  pinMode(RESET_SWITCH_Pin, OUTPUT);
  pinMode(DIN_SWITCH_Pin, OUTPUT);

  digitalWrite( SCLK_SWITCH_Pin, LOW );
  digitalWrite( DIN_SWITCH_Pin, LOW );
  digitalWrite( SYNC_SWITCH_Pin, HIGH );
  digitalWrite( RESET_SWITCH_Pin, HIGH );

  // Arduino handles most of the underlying SPI transfer by itself, set it up here
  SPI.begin();  // Enable SPI
  SPI.setBitOrder(MSBFIRST);  // Send data Most significant bit first, this is necessary for the AD9833
  SPI.setDataMode(SPI_MODE2);  // Set SPI Mode 2 (Data captured on falling edge of clock, clock inversion on)
  SPI.setClockDivider(SPI_CLOCK_DIV128);    // Set clock divider (optional)
}


void loop() {

  byte byteRead; // Serial read data

  String what_to_program; // Freq, Phase, All or Reset
  unsigned int chan_1_or_freq, chan_2;
  
  // Program DDS chips according to values in Freqs[].
  Program_Freqs (Freqs, NUM_CHANNELS);

  while (1) {
    // Wait for Serial data to be received and parse it
    //cycle_switches(2);
    //  switch_adg731(2, 19);

    //delay(ONE_SECOND_DELAY);
    //switch_adg731(2,20);

    if (Serial.available()) {

      what_to_program = Serial.readStringUntil(' '); // Get a string
      chan_1_or_freq = Serial.parseInt(); // Get an int
      chan_2 = Serial.parseInt(); // Get an int

      // Process input and call appropriate commands
      // Can't use switch statement to do this as it won't accept a string as a comparator
      if (what_to_program == "switch") {
        set_both_switches(chan_1_or_freq, chan_2);
      }

      else if (what_to_program == "freq") {
            Set_AD9833_Frequency(chan_1_or_freq, 1);
      }

      else if (what_to_program == "test") {
        test_channels();
      }

      // default case - invalid command has been passed
      else {
        Serial.println("Command not recognised.");
      }
    }
  }
}


void Program_Freqs (long Freqs [], int n_chans) {

  /* Loop through each freq/chan pair and program the switches/DDS chip */

  Serial.println("Programming default frequencies");
  for (int i = 0; i < n_chans; i++) {
    Set_AD9833_Frequency(Freqs[i], i + 1);
  }
}

// Additional functions that may be helpful

void Program_Then_Turn_Off(long Freqs[], int n_chans, unsigned int on_time_milli) {

  /* At the moment (6.12.16) we want to limit the injection time to some maximum value to be over-cautious.
    If the specified injection time is too long, cancel the injections by resetting all DDS chips and return */

  if (on_time_milli > MAX_CAUTIOUS_INJECTION) {
    Serial.println("Injection time higher than maximum defined time. Turning off all DDS chips");
    Reset_All(n_chans);
    return;
  }

  //Program all of the channels, wait a specified time, then turn off.
  else {
    Program_Freqs(Freqs, n_chans);
    delay(on_time_milli);
    Reset_All(n_chans);
  }
}

void switch_adg731(unsigned int switch_number, unsigned int chan_to_open) {
  // Open a particular channel on one of the switches
  // If chan_to_open == 0, close all switches

  Serial.print("Opening channel ");
  Serial.print(chan_to_open);
  Serial.print(" on switch ");
  Serial.println(switch_number);

  // Get the switch number to open, based on electrode number
  int switch_to_open = electrode_map_switch[switch_number - 1][chan_to_open - 1];
  
  // Set SYNC low
  set_adg731_sync(switch_number, 0);

  //Transfer 8 bits to open a switch, data format is
  // EN_ACTIVELOW CS_ACTIVELOW X A4,A3,A2,A1,A0
  // A4-A0 set the channel number (channel 1 = 00000)

  //Set EN_ACTIVELOW high to close all
  if (chan_to_open == 0) {
    SPI.transfer(128);
  }

  else {
    SPI.transfer(switch_to_open);
  }

  // Set SYNC high
  set_adg731_sync(switch_number, 1);

}

void set_adg731_sync(unsigned int switch_number, bool value) {
  // set the SYNC pin for a ADG731
  if (switch_number == 1) {
    digitalWrite(ADG731_SYNC1, value);
  }

  else if (switch_number == 2) {
    digitalWrite(ADG731_SYNC2, value);
  }
  else {
    return;
  }

}

void cycle_switches(unsigned int switch_num) {
  // Cycle through all channels and open each switch
  unsigned int num_switches = 32;
  
  for (int i = 0; i < num_switches; i++) {
    switch_adg731(switch_num, i);
    delay(500);
  }
}

void set_both_switches(unsigned int chan_1, unsigned int chan_2) {

  switch_adg731(1, chan_1);
  switch_adg731(2, chan_2);
}

void test_channels() {
// Test all channels
// Cycle through injections between channels 1-31 and 32. e.g. 1 & 32, 2 & 32, 3 & 32 etc
  Serial.println("Looping through all channels");
    
  int max_channels = 32;
  
  for (int i = 0; i < ( max_channels - 1) ; i ++) {

    set_both_switches(i, max_channels);
    delay(ONE_SECOND_DELAY);
   
  }
  
}

