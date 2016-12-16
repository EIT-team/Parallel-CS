# Parallel-CS
Parallel inject current source design

Includes: Circuit schematics, PCB Layout ,Arduino code for programming output frequency of CS

**Instructions for programming CS board**

Connect 2 x 3.7V Batteries to battery terminals.

Connect D-Sub connection for Arduino Nano.

Open /src/DDS_PCB/DDS_PCB.ino and upload to Arduino.

Devices will be programmed to default frequencies, specified in the code (long Freqs[8] = {1000, 2000, 1000, 1000, 1000, 1000, 1000, 1000}).

After programming, frequency and/or phase can be adjusted using serial monitor:
- Open serial monitor (Ctrl-Shift-M)
- Make sure line ending is set to '_no line ending_' and baud rate is 9600
- '_freq chan value_' will set the frequency of _chan_ to _value_ e.g. '_freq 2 2500_' sets channel 2 to 2500Hz
- '_phasechan value_' will set the phase in degrees of _chan_ to _value_ e.g. '_freq 2 50_' sets phase of channel 2 to 50 degrees. Phase is 0 by default.
- The terminal should echo a message confirming programming is happening

Once programmed, the D-Sub cable can be unplugged.

Amplitude can be adjusted by turning potentiometer. (Clockwise increases, anticlockwise decreases)


