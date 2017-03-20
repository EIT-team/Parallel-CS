#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "definitions.h"
#include "Arduino.h"
#include <iostream>

TEST_CASE("Set Phase") {
	
	unsigned int phase_word;
	
	int phases[] = {0, 90, 360, -1, 1000};
	int expected[] = {0xC000, 0xC400, 0xD000, 0xC000, 0xC000};
	int chan = 0;
	
	int num_test_phases = sizeof(phases) / sizeof(int);
	int i;
	
	for (i = 0; i < num_test_phases; i++) {
		
		phase_word = Set_AD9833_Phase(phases[i], chan);
		
		CHECK(phase_word == expected[i]);	
	}
	
}
