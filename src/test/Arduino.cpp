/* Mock Arduino functions */

#include "Arduino.h"
#include <iostream>

int digitalWrite(int pin, int value) {
	return value;
}

void delay(int delay_time) {
	;
}

// Mocked Serial commands

MockSerial::MockSerial() {
	;
}

void MockSerial::print(const char* buffer) {
	std::cout << buffer;
}

void MockSerial::println(const char* buffer) {
	std::cout << buffer << "\n";
}

void MockSerial::print(int num) {
	std::cout << num;
}

void MockSerial::println(int num) {
	std::cout << num << "\n";
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
