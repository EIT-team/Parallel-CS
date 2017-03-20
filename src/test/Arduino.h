/* Mock Arduino functions */

#ifndef _ArduinoH
#define _ArduinoH

// Arduino HIGH and LOW logic levels
#define HIGH 1
#define LOW 0

// Function declarations for built-in Arduino functions
void analogWrite(int, int);
int analogRead(int);
void delay(int);
void digitalWrite(int, int);

// Declarations for my Arduino functions
void Set_ADG984(int);
unsigned int Set_AD9833_Phase(int, int);
int Set_AD9833_Frequency(long, int);
unsigned long Get_Frequency_Word(long);
unsigned int Get_MSB(unsigned long);
unsigned int Get_LSB(unsigned long);


class MockSerial {
	// Mocks the Arduino Serial class
public:
	MockSerial();
	void print(char*);
	void println(char*);
	void print(int);
	void println(int);
};


extern MockSerial Serial;

class MockSPI {
	// Mocks the Arduino SPI class

public:
	MockSPI();
	void begin();
	unsigned int transfer(unsigned int);

};

extern MockSPI SPI;

#endif
