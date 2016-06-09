/*Program the AD9833, see http://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf for more details
  Calculates register values needed to program specific frequency and then uses AD9833_SendWord function to actually program the chip using SPI*/

//

#define DELAY_TIME 100    //Time to delay after programming switches
#define CONTROL_REGISTER_VALUE 0x2000    //Default value for control register of DDS chip
#define PHASE_REGISTER_VALUE 0xC000      //Default value for phase register of DDS chip
#define RESET_CONTROL_REGISTER 0x2100    //Reset control register of DDS to produce midscale output


void Set_AD9833_Frequency(int freq, unsigned long F_MCLK, int chan) {
 

  // Generating the frequency registers from the desired frequency
  unsigned long freq_word = (unsigned long)(0x10000000 / F_MCLK * freq); //frequency value to send to AD9833, needs to be separated into two parts, 14 bits long each
  unsigned int msb = (freq_word >> 14); //4 MSB of long data type aren't used, this gets the next 14 MSB
  unsigned int lsb = (freq_word & 0x3fff); // Gives the 14 LSB

  //Now we have two sets of 14 bits. Each word to be sent to the AD9833 should be 16 bits, with the 2 MSB indicating which register on the chip to send to
  //Here, using Register 0 (set 2 MSB to 01). To use Register 1, set 2MSB to 10 ( msb | 0x8000)
  msb = msb | 0x4000;
  lsb = lsb | 0x4000;

  //Set control register, frequency register (in two parts LSB and MSB) and phase register
  AD9833_SendWord(CONTROL_REGISTER_VALUE, chan);       //Control register
  AD9833_SendWord(lsb, chan);                         //Frequency Regsiter part 1 (LSB)
  AD9833_SendWord(msb, chan);                         //Frequency Register part 2 (MSB)
  AD9833_SendWord(PHASE_REGISTER_VALUE, chan);        //Phase regsister, don't need to change this at the moment, so set to 0 phase

} 


void AD9833_SendWord(unsigned int data, int chan) {
  /*SPI Data write
  Can only send 8 bits at a time, so this splits up a 16 bit word and sends it as two parts
  Add in several ms delay before/after SPI transfer to account for propagation delay differences between digital isolators and switching circuitry */

  //Set FSYNC pin on the DDS chip we want to program
  Set_ADG984(chan);
  delay(DELAY_TIME);

  //Send 16 bit word as two 8 bit sections
  SPI.transfer((data >> 8) & 0xFF);
  SPI.transfer(data & 0xFF);

  //Disable SPI
  //We want to close all of the switches, so use some sentinel values >> than the number of switches (1000) to make this happen
  Set_ADG984(1000);
  delay(DELAY_TIME);


}


//Sweeps the frequency output on a channel, with increment and max value set by user.

void Sweep_Freq (int freq_min, int freq_step, int freq_max, int chan, int delay_time) {
  
    unsigned long  F_MCLK = DDS_CLOCK_FREQUENCY;

  for (int i = freq_min ; i <=freq_max;i = i + freq_step)
  {

        Set_AD9833_Frequency(500, F_MCLK, 2);
  Set_AD9833_Frequency(2500, F_MCLK, 3);
  

   Set_AD9833_Frequency(i, F_MCLK, chan);
   delay(delay_time);
     Reset_DDS(2);
    Reset_DDS(3);
    
    delay(delay_time); 
    
  }
  
}

//Resets all channels to default output (essentially off)
void Reset_DDS(int chan) {

AD9833_SendWord(RESET_CONTROL_REGISTER, chan); 
}


void Reset_All(int n_chans) {

  for (int j = 1; j <= n_chans  ; j++) {
  Reset_DDS(j);
}
}




