
void Set_AD9833_Frequency(int freq, unsigned long F_MCLK, int chan) {
  
  /*Program the AD9833, see http://www.analog.com/media/en/technical-documentation/application-notes/AN-1070.pdf for more details
  Calculates register values needed to program specific frequency and then uses AD9833_SendWord function to actually program the chip using SPI*/
 
 
   // Code for generating the frequency registers
  unsigned long freq_word = (unsigned long)(0x10000000 / F_MCLK * freq); //frequency value to send to AD9833, needs to be separated into two parts
  unsigned int msb = (freq_word >> 14); //4 MSB of long are 0, this gets the next 14 MSB
  unsigned int lsb = (freq_word & 0x3fff); // Gives the 14 LSB

  //Now we have two sets of 14 bits. Each word to be sent to the AD9833 should be 16 bits, with the 2 MSB indicating which register to send to
  //Here, using Register 0 (set 2 MSB to 01). To use Register 1, set 2MSB to 10 ( msb | 0x8000)
  msb = msb | 0x4000;
  lsb = lsb | 0x4000;

  /*Serial.print(msb,HEX);
  Serial.print("\n");
  Serial.print(lsb,HEX);
  Serial.print("\n");
  */
  //Frequency is controlled by a 28 bit word, which has to be inputted in two parts
 Set_ADG984(chan);
delay(1);
  AD9833_SendWord(0x2000); //Control register
   delay(1);
 Set_ADG984(chan);
delay(1);
  AD9833_SendWord(lsb); //Frequency Regsiter part 1 (LSB)
  delay(1);
 Set_ADG984(chan);
 delay(1);
  AD9833_SendWord(msb);  //Frequency Register part 2 (MSB)
  delay(1);
 Set_ADG984(chan);
 delay(1);
  AD9833_SendWord(0xC000);  //Phase regsister, don't need to change this at the moment, so set to 0 phase
  delay(1);

  //AD9833_SendWord(0x2000);  //Exit reset, sets the device to run
delay(1);

}



void AD9833_SendWord(unsigned int data) {
/*SPI Data write
Can only send 8 bits at a time, so this splits up a 16 bit word and sends it as two parts*/
  
  //enable SPI on Arduino
 // digitalWrite(FSYNC_Pin, LOW);

  //Send 16 bit word as two 8 bit sections
  SPI.transfer((data >> 8) & 0xFF);
  SPI.transfer(data & 0xFF);

  //Disable SPI 
   Set_ADG984(8);

  //digitalWrite(FSYNC_Pin, HIGH);
}

