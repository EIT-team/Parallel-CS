test_file: test.o Program_Switch.o Arduino.o Program_DDS.o
	g++ -o test_file test.o Program_Switch.o Arduino.o Program_DDS.o

Program_DDS.o: DDS_PCB/Program_DDS.ino Arduino.o Program_Switch.o test/Arduino.h
	g++ -c -x c++ DDS_PCB/Program_DDS.ino -Itest/
	
Program_Switch.o: DDS_PCB/Program_Switch.ino Arduino.o test/Arduino.h
	g++ -c -x c++ DDS_PCB/Program_Switch.ino -Itest/
	
test.o: test/test.cpp  DDS_PCB/definitions.h test/Arduino.h
	g++ -c test/test.cpp -IDDS_PCB/

Arduino.o: test/Arduino.cpp
	g++ -c test/Arduino.cpp
	
