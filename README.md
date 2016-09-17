# DSK Synthesizer
C source code for a simple(monophonic) playback synthesizer, implemented on the TI DSK6713 signal processor. It operates by taking an encoded musical score and then playing it through the onboard DAC. Notes are produced by reading 1 of 12 sine tables from memory. A sample musical score(Ode to Joy) is included for testing. 

#Usage
Playback is achieved by loading a text file containing encoded representations of notes and their durations. Encoding is achieved as follows:  

Each line within the text file represents a note and its attributes. It consists of a tuple of 4 int values. The first is a number in the range of 0 to 12 that represents a musical note where:
```
C = 0;
C# = 1;
D = 2;
D# = 3;
E = 4;
F = 5;
F# = 6;
G = 7;
G# = 8;
A = 9;
A# = 10;
B = 11;
Rest = 12;
```
The second value represents a duration for which a note is to be played. Note convention in this project is as follows
```
whole note = 40000;
half note = 20000;
quarter note = 10000; 
eighth note = 5000;  
sixteenth note = 2500;    
thirty secondth note = 1250;      
sixty fourth note = 625;
```
The third value corresponds to how many octaves a note should be played above the default range (middle c in this case), and the last value represents a legato (1 for legato, 0 for no legato).
