#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "definitions.h"
#include "Arduino.h"
#include <iostream>
	
	TEST_CASE("Set Phase") {
	
	unsigned int phase_word;
	
	int phase = 0;
	int chan = 0;
	
	phase_word = Set_AD9833_Phase(phase, chan);
	
	CHECK(phase_word == 0xC000);
	}
