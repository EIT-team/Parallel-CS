// There isnt a direct mapping of electrode numbers to pins on the switches, due to routing constraints on the PCB
// These arrays map the electrode number to the correct switch index i.e. switch_1_map[5] gives the switch number
// for electrode 5.

#include "definitions.h"
#include "Arduino.h"
#include <stdio.h>



void Program_ADG731 (int S1, int S2) {

  unsigned int switch_1_map [33] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 32, 31, 17, 18, 19, 29, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
  unsigned int switch_2_map [33] = {0, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 21, 22, 23, 24, 25, 26, 13, 14, 15, 16, 32, 31, 30, 29, 20, 19, 18, 17, 27, 28};

  unsigned int S1_actual_pin = switch_1_map[S1];
  unsigned int S2_actual_pin = switch_1_map[S2];

  //Calculate data word

  unsigned int data_word_1 = ( S1_actual_pin >> 3 ) & ADG731_ENABLE_MASK;
  unsigned int data_word_2 = ( S2_actual_pin >> 3 ) & ADG731_ENABLE_MASK;

  Serial.println(data_word_1);
  Serial.println(data_word_2);
  
  // Write switch 1
  digitalWrite(SYNC1_Pin, LOW);
  ADG731_SendWord(data_word_1);
  digitalWrite(SYNC1_Pin, HIGH);

  digitalWrite(SYNC2_Pin, LOW);
  ADG731_SendWord(data_word_2);
  digitalWrite(SYNC2_Pin, HIGH);
}

void ADG731_SendWord(unsigned int data) {

  // Use Bit Mask to only send the 8 MSB
    SPI.transfer(data & SPI_BIT_MASK);


}

