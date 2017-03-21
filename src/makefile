CC=g++ 
CFLAGS=-c -Wall
ARDFLAGS=-x c++ # Flags when compiling Arduino files with .ino extension
INC_ARD=-I./DDS_PCB/
INC_TEST=-I./test/ 

all: main

main: test.o Program_Switch.o Arduino.o Program_DDS.o
	$(CC) -o test_file test.o Program_Switch.o Arduino.o Program_DDS.o

Program_DDS.o: DDS_PCB/Program_DDS.ino Arduino.o Program_Switch.o test/Arduino.h
	$(CC) $(CFLAGS) $(ARDFLAGS) DDS_PCB/Program_DDS.ino $(INC_TEST)
	
Program_Switch.o: DDS_PCB/Program_Switch.ino Arduino.o test/Arduino.h
	$(CC) $(CFLAGS) $(ARDFLAGS) DDS_PCB/Program_Switch.ino $(INC_TEST)
	
test.o: test/test.cpp  DDS_PCB/definitions.h test/Arduino.h
	$(CC) $(CFLAGS) test/test.cpp $(INC_ARD)

Arduino.o: test/Arduino.cpp
	$(CC) $(CFLAGS) test/Arduino.cpp

clean:
	rm test.o Program_Switch.o Arduino.o Program_DDS.o
