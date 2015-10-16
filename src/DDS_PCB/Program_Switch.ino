/* Opens a single switch on the ADG984, to allow FSYNC to be routed to the appropriate DDS IC */

void Set_ADG984 (int channel_to_open) {

  //Reset switches

  digitalWrite(RESET_SWITCH_Pin, LOW);
  digitalWrite(RESET_SWITCH_Pin, HIGH);

   //Program the switches
  //Set SYNC low, clock through switch setting values (0 or 1) , then set SYNC high to actually open/close switches


    //Set SYNC low to enable programming of switches
    digitalWrite(SYNC_SWITCH_Pin, LOW);

    //Iterate 8 times, once for each switch. Turn on switches based on values in Injection[][]
    //1st command sent is for switch 40, last is for switch 1.
    for (int j = 8 ; j > 0 ; j--) {

      if (j == channel_to_open) 
     { digitalWrite(DIN_SWITCH_Pin, LOW);}

     else
{      digitalWrite(DIN_SWITCH_Pin, HIGH);}



      //Generate clock pulse to clock in switch values to DIN pins.
      digitalWrite(SCLK_SWITCH_Pin, HIGH);
      digitalWrite(SCLK_SWITCH_Pin, LOW);
      
     digitalWrite(DIN_SWITCH_Pin, LOW);
     


    }
    //Set SYNC high to update switches
    digitalWrite(SYNC_SWITCH_Pin, HIGH);
  


}

