# Parallel-CS
Parallel inject current source design

Includes:

Circuit schematics

PCB Layout

Arduino code for programming output frequency of DDS chip (/src/DDS_PCB)

**Instructions for programming CS board**

Clone Repository, open /src/DDS_PCB/DDS_PCB.ino and upload to Arduino Nano
Devices will be programmed to default frequencies, specified in the code (long Freqs[8] = {1000, 2000, 1000, 1000, 1000, 1000, 1000, 1000})
After programming, frequency and/or phase can be adjusted using serial monitor:
-Open serial monitor (Ctrl-Shift-M)
-Make sure line ending is set to '__no line ending__' and baud rate is 9600
-'__freq chan value__' will set the frequency of __chan__ to __value__ e.g. '__freq 2 2500__' sets channel 2 to 2500Hz
-The terminal should echo a message confirming programming is happening


