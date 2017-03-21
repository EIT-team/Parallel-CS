/* Mock Arduino functions and constants when compiling stand along code for testing */

#ifndef arduino_h
#define arduino_h

// Arduino HIGH and LOW logic levels
#define HIGH 1
#define LOW 0

// Function declarations for built-in Arduino functions
void delay(int);
int digitalWrite(int, int);

// Declarations for my Arduino functions
// Standard Arduino compiler ignores/doesn't need declarations
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
	void print(const char*);
	void println(const char*);
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
