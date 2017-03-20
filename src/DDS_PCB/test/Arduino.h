/* Mock Arduino functions */

#ifndef _ArduinoH
#define _ArduinoH

#define HIGH 1
#define LOW 0

// Function declarations
void analogWrite(int, int);
int analogRead(int);
void delay(int);
void digitalWrite(int, int);


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
	;
};

#endif
