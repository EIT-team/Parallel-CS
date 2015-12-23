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

//Library for handling the SPI transfer
#include <SPI.h>

//Variable for the SPI Enable pin
#define FSYNC_Pin 5
#define SDATA_SPI_Pin 11
#define SCLK_SPI_Pin  13

#define SCLK_SWITCH_Pin 9
#define SYNC_SWITCH_Pin 2
#define RESET_SWITCH_Pin 3
#define DIN_SWITCH_Pin 4

//Clock generator frequency, set by resistor R_SET on the PCB. This is used in the formula for setting the DDS sine wave frequency
#define DDS_CLOCK_FREQUENCY 10e6

//*/*/*
//##############################CHANGE FREQUENCIES HERE################
int Freqs[8] = {6000,10000,14000,18000,22000,26000,30000,34000};
//#######################################
//*/*/*/

void setup() {

  Serial.begin(9600);

  // For SPI data transfer setup

  pinMode(FSYNC_Pin, OUTPUT);
  pinMode(SDATA_SPI_Pin, OUTPUT);
  pinMode(SCLK_SPI_Pin, OUTPUT);


  digitalWrite(FSYNC_Pin, HIGH); //Set FSYNC High. FSYNC is active low
  digitalWrite(SDATA_SPI_Pin, LOW); //Set SDATA Low
  digitalWrite(SCLK_SPI_Pin, LOW); //Set SCLK Low

  //For Digital Switch
  pinMode (SCLK_SWITCH_Pin, OUTPUT);
  pinMode (SYNC_SWITCH_Pin, OUTPUT);
  pinMode (RESET_SWITCH_Pin, OUTPUT);
  pinMode (DIN_SWITCH_Pin, OUTPUT);

  digitalWrite ( SCLK_SWITCH_Pin, LOW );
  digitalWrite ( DIN_SWITCH_Pin, LOW );
  digitalWrite ( SYNC_SWITCH_Pin, HIGH );
  digitalWrite ( RESET_SWITCH_Pin, HIGH );


  //Arduino handles most of the underlying SPI transfer by itself, set it up here
  SPI.begin();  //Enable SPI
  SPI.setBitOrder(MSBFIRST);  //Send data Most significant bit first, this is necessary for the AD9833
  SPI.setDataMode(SPI_MODE2);  //Set SPI Mode 2 (Data captured on falling edge of clock, clock inversion on)
  SPI.setClockDivider(SPI_CLOCK_DIV128);    //  Set clock divider (optional)

}



void loop() {


unsigned long  F_MCLK = DDS_CLOCK_FREQUENCY;

 
   
  int n_chans = sizeof(Freqs)/sizeof(int);
  
  //Test_Single_Chan(Freqs,8,2);
  
  Program_Freqs (Freqs,n_chans);




delay(1000);

  // Frequency Sweep
  /*Reset_All(n_chans);
  delay(10000);
  
  
  Sweep_Freq(500,100,3000,1,10000);
  
  Reset_All(n_chans);
  delay(50000);
*/
//Idle loop
 // while (1) {
  //}



}

void Program_Freqs (int Freqs [], int n_chans) {
 
  unsigned long  F_MCLK = DDS_CLOCK_FREQUENCY;


Serial.print(n_chans);
Serial.print('\n');
//Loop through each freq/chan pair and program the switches/DDS chip


  for (int i = 0; i < n_chans; i++) {
    Serial.print(i);
    Serial.print('\n');

    Set_AD9833_Frequency(Freqs[i], F_MCLK, i+1);
   
  }
  
}


void Test_Single_Chan (int Freqs [], int n_freqs, int chan) {
    unsigned long  F_MCLK = DDS_CLOCK_FREQUENCY;
Serial.print("Single Chan Sweep");

  for (int i = 0; i < n_freqs; i++) {

      Set_AD9833_Frequency(Freqs[i], F_MCLK, chan);
  delay(2500);
  }
  
}
