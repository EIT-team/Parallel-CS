/* Mock Arduino functions */

#include "Arduino.h"

void analogWrite(int pin, int value) {
	;
}

void digitalWrite(int pin, int value) {
	;
}

int analogRead(int pin) {
	return pin;
}

void delay(int delay_time) {
	;
}

// Mocked Serial commands

MockSerial::MockSerial() {
	;
}

void MockSerial::print(char* buffer) {
	;
}

void MockSerial::println(char* buffer) {
	;
}

void MockSerial::print(int num) {
	;
}

void MockSerial::println(int num) {
	;
}

MockSerial Serial;

// Mocked SPI commands

MockSPI::MockSPI() {
	;
}

void MockSPI::begin() {
	;
}

unsigned int MockSPI::transfer(unsigned int data) {
	return data;
}

MockSPI SPI;
