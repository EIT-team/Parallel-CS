[![Build Status](https://travis-ci.org/EIT-team/Parallel-CS.svg?branch=master)](https://travis-ci.org/EIT-team/Parallel-CS)

# Parallel-CS
Parallel inject current source design

Includes: Circuit schematics, PCB Layout, Arduino code for programming output frequency and/pr phase of CS.

**Instructions for programming CS board**

Connect mini-USB cable to Arduino.

Open /src/DDS_PCB/DDS_PCB.ino and upload to Arduino.

Devices will be initially be programmed to default frequencies, specified in the src/frequencies.h file.

After programming, frequency and/or phase can be adjusted using serial monitor:
- Open serial monitor (Ctrl-Shift-M)
- Make sure line ending is set to '_no line ending_' and baud rate is 9600
- '_freq chan value_' will set the frequency of _chan_ to _value_ e.g. '_freq 2 2500_' sets channel 2 to 2500Hz
- '_phase chan value_' will set the phase in degrees of _chan_ to _value_ e.g. '_phase 2 50_' sets phase of channel 2 to 50 degrees. Phase is 0 by default.
- The terminal should echo a message confirming programming is happening.

Once programmed, the USB cable can be unplugged.



