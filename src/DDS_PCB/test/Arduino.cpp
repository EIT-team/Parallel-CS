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

void MockSerial::print(char*) {
	;
}

void MockSerial::println(char*) {
	;
}

void MockSerial::print(int) {
	;
}

void MockSerial::println(int) {
	;
}

MockSerial Serial;

