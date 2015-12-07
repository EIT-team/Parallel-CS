/*
 * 
 * DDS Sine Generator using Arduino Nano PWM
 * Interrupt timer is used to control the PWM wave
 * External Low pass filter needed on PWM output to generate square wave
 */

#include "avr/pgmspace.h"

// table of 256 sine values / one sine period / stored in flash memory
PROGMEM  prog_uchar sine256[]  = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
  221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
  76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
  33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124

};
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

int ledPin = 13;                 // LED pin 7
int testPin = 7;
int t2Pin = 6;
byte bb;

double dfreq;
double dfreqB;
double dfreqC;
double dfreqD;

// const double refclk=31372.549;  // =16MHz / 510
const double refclk=31376.6;      // measured

// variables used inside interrupt service declared as voilatile
volatile byte icnt;              // var inside interrupt
volatile byte icntB;
volatile byte icnt1;             // var inside interrupt
volatile byte c4ms;              // counter incremented all 4ms
volatile byte c4msB;
volatile unsigned long phaccu;   // pahse accumulator
volatile unsigned long tword_m;  // dds tuning word m
volatile unsigned long phaccuB;   // pahse accumulator
volatile unsigned long tword_mB;  // dds tuning word m

volatile byte icntC;              // var inside interrupt
volatile byte icntD;
volatile byte c4msC;              // counter incremented all 4ms
volatile byte c4msD;
volatile unsigned long phaccuC;   // pahse accumulator
volatile unsigned long tword_mC;  // dds tuning word m
volatile unsigned long phaccuD;   // pahse accumulator
volatile unsigned long tword_mD;  // dds tuning word m

volatile byte icntE;              // var inside interrupt
volatile byte icntF;
volatile byte c4msE;              // counter incremented all 4ms
volatile byte c4msF;
volatile unsigned long phaccuE;   // pahse accumulator
volatile unsigned long tword_mE;  // dds tuning word m
volatile unsigned long phaccuF;   // pahse accumulator
volatile unsigned long tword_mF;  // dds tuning word m

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets he digital pin as output
  Serial.begin(115200);        // connect to the serial port
  Serial.println("DDS Test");
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);      // sets the digital pin as output
  pinMode(7, OUTPUT);      // sets the digital pin as output
  
  //These pins are used for the timer interrupt outputs
  //Timer 1
  pinMode(9, OUTPUT);     // pin11= PWM  output / frequency output
  pinMode(10, OUTPUT);
  //Timer 2
  pinMode(11, OUTPUT);     // pin11= PWM  output / frequency output
  pinMode(3, OUTPUT);
 
 
  Setup_timer1(); 
  Setup_timer2();

  // disable interrupts to avoid timing distortion
  sbi (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available
  sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt
  sbi (TIMSK1,TOIE1);              // enable Timer1 Interrupt

  dfreq=1000.0;                    // initial output frequency = 1000.o Hz
  tword_m=pow(2,32)*dfreq/refclk;  // calulate DDS new tuning word 
  
  dfreqB =2000.0;
  tword_mB = pow(2,32)*dfreqB/refclk;
  
  dfreqC =4000.0;
  tword_mC = pow(2,32)*dfreqC/refclk;
  
  dfreqD =500.0;
  tword_mD = pow(2,32)*dfreqD/refclk;

}
void loop()
{
  while(1) {
//     if (c4ms > 250) {                 // timer / wait fou a full second
//      c4ms=0;
//    
//
//      Serial.print(dfreq);
//      Serial.print("  ");
//      Serial.println(tword_m);
//    }

   sbi(PORTD,6); // Test / set PORTD,7 high to observe timing with a scope
   cbi(PORTD,6); // Test /reset PORTD,7 high to observe timing with a scope
  }
 }
 



//******************************************************************
// timer1 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer1() {
//
// Timer2 Clock Prescaler to : 1
  sbi (TCCR1B, CS20);
  cbi (TCCR1B, CS21);
  cbi (TCCR1B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  cbi (TCCR1A, COM2A0);  // clear Compare Match
  sbi (TCCR1A, COM2A1);
  sbi( TCCR1A, COM2B1);

  sbi (TCCR1A, WGM20);  // Mode 1  / Phase Correct PWM
  
  cbi (TCCR1A, WGM21);
  cbi (TCCR1B, WGM22);
}



//******************************************************************
// timer2 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer2() {

// Timer2 Clock Prescaler to : 1
  sbi (TCCR2B, CS20);
  cbi (TCCR2B, CS21);
  cbi (TCCR2B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  cbi (TCCR2A, COM2A0);  // clear Compare Match
  sbi (TCCR2A, COM2A1);
  sbi( TCCR2A, COM2B1);

  sbi (TCCR2A, WGM20);  // Mode 1  / Phase Correct PWM
  
  cbi (TCCR2A, WGM21);
  cbi (TCCR2B, WGM22);
}



//******************************************************************
// Timer2 Interrupt Service at 31372,550 KHz = 32uSec
// this is the timebase REFCLOCK for the DDS generator
// FOUT = (M (REFCLK)) / (2 exp 32)
// runtime : 8 microseconds ( inclusive push and pop)
ISR(TIMER2_OVF_vect) {

  sbi(PORTD,7);          // Test / set PORTD,7 high to observe timing with a oscope

  phaccuC=phaccuC+tword_mC; // soft DDS, phase accu with 32 bits
  icntC=phaccuC >> 24;     // use upper 8 bits for phase accu as frequency information
                         // read value fron ROM sine table and send to PWM DAC

  phaccuD=phaccuD+tword_mD; // soft DDS, phase accu with 32 bits
  icntD=phaccuD >> 24;     // use upper 8 bits for phase accu as frequency information
                         // read value fron ROM sine table and send to PWM DAC
  OCR2B=pgm_read_byte_near(sine256 + icntC);    
  OCR2A=pgm_read_byte_near(sine256 + icntD);

//  if(icnt1++ == 125) {  // increment variable c4ms all 4 milliseconds
//    c4ms++;
//    icnt1=0;
//   }   

 cbi(PORTD,7);            // reset PORTD,7
}

//******************************************************************
// Timer1 Interrupt Service at 31372,550 KHz = 32uSec
// this is the timebase REFCLOCK for the DDS generator
// FOUT = (M (REFCLK)) / (2 exp 32)
// runtime : 8 microseconds ( inclusive push and pop)
ISR(TIMER1_OVF_vect) {


  phaccu=phaccu+tword_m; // soft DDS, phase accu with 32 bits
  icnt=phaccu >> 24;     // use upper 8 bits for phase accu as frequency information
                         // read value fron ROM sine table and send to PWM DAC

  phaccuB=phaccuB+tword_mB; // soft DDS, phase accu with 32 bits
  icntB=phaccuB >> 24;     // use upper 8 bits for phase accu as frequency information
                         // read value fron ROM sine table and send to PWM DAC
  OCR1B=pgm_read_byte_near(sine256 + icnt);    
  OCR1A=pgm_read_byte_near(sine256 + icntB);

//  if(icnt1++ == 125) {  // increment variable c4ms all 4 milliseconds
//    c4ms++;
//    icnt1=0;
//   }   

}


