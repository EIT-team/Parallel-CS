#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "definitions.h"
#include "Arduino.h"
#include <iostream>

TEST_CASE("Set Phase") {
	
	unsigned int phase_word;
	
	// Valid phases are in range 0-360.
	int phases[] = {0, 90, 360, -1, 1000};
	int expected[] = {0xC000, 0xC400, 0xD000, 0xC000, 0xC000};
	int chan = 1;
	
	int num_test_phases = sizeof(phases) / sizeof(int);
	int i;
	
	for (i = 0; i < num_test_phases; i++) {
		
		phase_word = Set_AD9833_Phase(phases[i], chan);
		
		CHECK(phase_word == expected[i]);	
	}
}

TEST_CASE("Set Frequency") {
	
	// Which channel to program, use in all test sections
	int chan = 1;
	
	SECTION ("Check rejection of invalid frequencies") {
		
		// Try and program a range of invalid frequency values (< 0, 0, > 100kHz)
		
		int invalid_freqs[] = {-1, 0, 100001};
		int num_freqs = sizeof(invalid_freqs) / sizeof(int);
		int expected = -1;
		int i;
		
		for (i = 0; i < num_freqs; i++) {
			int answer = Set_AD9833_Frequency(invalid_freqs[i], chan);
			CHECK(answer == expected);	
		}
	}
	
	int valid_freq = 1000;

	SECTION("Check valid frequency accepted") {
		
		int expected = 1;
		
		int answer = Set_AD9833_Frequency(valid_freq, chan);
		CHECK(answer == expected);
	}
	
	SECTION("Check frequency word") {
		
		// Test that the correct frequency word, MSB and LSB are being generated.
		unsigned long freq_word = Get_Frequency_Word(valid_freq);
		unsigned int msb = Get_MSB(freq_word);
		unsigned int lsb = Get_LSB(freq_word);
		
		unsigned long expected_word = 0x68DB;
		unsigned int expected_msb = 0x4001;
		unsigned int expected_lsb = 0x68DB;
		
		CHECK(freq_word == expected_word);
		CHECK(msb == expected_msb);
		CHECK(lsb == expected_lsb);
	}
	
}