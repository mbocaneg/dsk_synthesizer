#include <stdio.h>
#include "dsk6713_aic23.h"                  //support file for codec,DSK

Uint32 fs = DSK6713_AIC23_FREQ_16KHZ;        //set sampling rate to 16 KHZ

//buffer lengths
#define BufferC 122
#define BufferCs 115
#define BufferD 109
#define BufferDs 103

#define BufferE 98
#define BufferF 92
#define BufferFs 86
#define BufferG 82
#define BufferGs 77

#define BufferA 73
#define BufferAs 69
#define BufferB 65

FILE *fin;

//notes, which are encoded as a number from 0 to 12

int noteC = 0;
int noteCs = 1;
int noteD = 2;
int noteDs = 3;
int noteE = 4;
int noteF = 5;
int noteFs = 6;
int noteG = 7;
int noteGs = 8;
int noteA = 9;
int noteAs = 10;
int noteB = 11;
int noteRest = 12;

//note durations
int whole = 40000;
int half = 20000;
int quarter = 10000; 
int eighth = 5000;  
int sixteenth = 2500;    
int thirty_secondth = 1250;      
int sixty_fourth = 625;
int thisOctave = 1;

//sine tables of each note

short tableC[BufferC] = {0, 515, 1028, 1539, 2046, 2547, 3041, 3528, 4005, 4471, 4925, 5367, 5794, 
6206, 6602, 6979, 7339, 7679, 7998, 8297, 8573, 8827, 9057, 9263, 9445, 9601, 9733, 9838, 9917, 9970, 
9997, 9997, 9970, 9917, 9838, 9733, 9601, 9445, 9263, 9057, 8827, 8573, 8297, 7998, 7679, 7339, 6979,
 6602, 6206, 5794, 5367, 4925, 4471, 4005, 3528, 3041, 2547, 2046, 1539, 1028, 515, 0, -515, -1028, 
 -1539, -2046, -2547, -3041, -3528, -4005, -4471, -4925, -5367, -5794, -6206, -6602, -6979, -7339, -7679,
  -7998, -8297, -8573, -8827, -9057, -9263, -9445, -9601, -9733, -9838, -9917, -9970, -9997, -9997, -9970,
   -9917, -9838, -9733, -9601, -9445, -9263, -9057, -8827, -8573, -8297, -7998, -7679, -7339, -6979, -6602,
    -6206, -5794, -5367, -4925, -4471, -4005, -3528, -3041, -2547, -2046, -1539, -1028, -515};
    
short tableCs[BufferCs] = {0, 546, 1091, 1632, 2168, 2698, 3220, 3732, 4233, 4721, 5196, 5655, 6097, 
6520, 6925, 7308, 7670, 8009, 8324, 8614, 8879, 9117, 9328, 9511, 9665, 9791, 9887, 9954, 9992, 9999, 
9977, 9925, 9843, 9732, 9591, 9423, 9226, 9001, 8750, 8472, 8170, 7843, 7492, 7119, 6725, 6311, 5878, 
5427, 4961, 4479, 3984, 3477, 2960, 2434, 1901, 1362, 819, 273, -273, -819, -1362, -1901, -2434, -2960, 
-3477, -3984, -4479, -4961, -5427, -5878, -6311, -6725, -7119, -7492, -7843, -8170, -8472, -8750, -9001, 
-9226, -9423, -9591, -9732, -9843, -9925, -9977, -9999, -9992, -9954, -9887, -9791, -9665, -9511, -9328, 
-9117, -8879, -8614, -8324, -8009, -7670, -7308, -6925, -6520, -6097, -5655, -5196, -4721, -4233, -3732, 
-3220, -2698, -2168, -1632, -1091, -546};

short tableD[BufferD] = {0, 576, 1150, 1721, 2285, 2842, 3390, 3926, 4450, 4958, 5450, 5924, 6379, 6812, 
7222, 7609, 7970, 8305, 8612, 8890, 9139, 9358, 9546, 9701, 9825, 9916, 9974, 9999, 9991, 9949, 9875, 9767, 
9627, 9456, 9253, 9019, 8755, 8462, 8141, 7793, 7419, 7020, 6598, 6154, 5690, 5207, 4706, 4190, 3660, 3118, 
2565, 2004, 1436, 864, 288, -288, -864, -1436, -2004, -2565, -3118, -3660, -4190, -4706, -5207, -5690, -6154, 
-6598, -7020, -7419, -7793, -8141, -8462, -8755, -9019, -9253, -9456, -9627, -9767, -9875, -9949, -9991, -9999, 
-9974, -9916, -9825, -9701, -9546, -9358, -9139, -8890, -8612, -8305, -7970, -7609, -7222, -6812, -6379, -5924,
 -5450, -4958, -4450, -3926, -3390, -2842, -2285, -1721, -1150, -576};
 
short tableDs[BufferDs] = {0, 610, 1217, 1820, 2416, 3003, 3579, 4142, 4689, 5218, 5729, 6218, 6684, 7125, 7539, 7926, 8283, 8609, 8903, 9164, 9391, 9583, 9739, 9860, 9943, 9990, 9999, 9971, 9906, 9804, 9666, 9492, 9282, 9038, 8760, 8450, 8108, 7736, 7335, 6907, 6454, 5976, 5476, 4956, 4417, 3862, 3293, 2711, 2119, 1519, 914, 305, -305, -914, -1519, -2119, -2711, -3293, -3862, -4417, -4956, -5476, -5976, -6454, -6907, -7335, -7736, -8108, -8450, -8760, -9038, -9282, -9492, -9666, -9804, -9906, -9971, -9999, -9990, -9943, -9860, -9739, -9583, -9391, -9164, -8903, -8609, -8283, -7926, -7539, -7125, -6684, -6218, -5729, -5218, -4689, -4142, -3579, -3003, -2416, -1820, -1217, -610};

short tableE[BufferE] = {0, 641, 1279, 1912, 2537, 3151, 3753, 4339, 4907, 5455, 5981, 6482, 6957, 7403, 
7818, 8202, 8551, 8866, 9144, 9385, 9587, 9749, 9872, 9954, 9995, 9995, 9954, 9872, 9749, 9587, 9385, 
9144, 8866, 8551, 8202, 7818, 7403, 6957, 6482, 5981, 5455, 4907, 4339, 3753, 3151, 2537, 1912, 1279, 
641, 0, -641, -1279, -1912, -2537, -3151, -3753, -4339, -4907, -5455, -5981, -6482, -6957, -7403, -7818, 
-8202, -8551, -8866, -9144, -9385, -9587, -9749, -9872, -9954, -9995, -9995, -9954, -9872, -9749, -9587, 
-9385, -9144, -8866, -8551, -8202, -7818, -7403, -6957, -6482, -5981, -5455, -4907, -4339, -3753, -3151, 
-2537, -1912, -1279, -641};

short tableF[BufferF] = {0, 682, 1362, 2035, 2698, 3349, 3984, 4601, 5196, 5767, 6311, 6826, 7308, 7757, 8170, 
8544, 8879, 9172, 9423, 9629, 9791, 9907, 9977, 10000, 9977, 9907, 9791, 9629, 9423, 9172, 8879, 8544, 
8170, 7757, 7308, 6826, 6311, 5767, 5196, 4601, 3984, 3349, 2698, 2035, 1362, 682, 0, -682, -1362, 
-2035, -2698, -3349, -3984, -4601, -5196, -5767, -6311, -6826, -7308, -7757, -8170, -8544, -8879, 
-9172, -9423, -9629, -9791, -9907, -9977, -10000, -9977, -9907, -9791, -9629, -9423, -9172, -8879, 
-8544, -8170, -7757, -7308, -6826, -6311, -5767, -5196, -4601, -3984, -3349, -2698, -2035, -1362, -682};


short tableFs[BufferFs] = {0, 730, 1456, 2174, 2881, 3572, 4245, 4894, 5518, 6112, 6673, 7199, 7686, 8133, 8536, 8893, 9203, 9464, 9675, 9834, 9940, 9993, 9993, 9940, 9834, 9675, 9464, 9203, 8893, 8536, 8133, 7686, 7199, 6673, 6112, 5518, 4894, 4245, 3572, 2881, 2174, 1456, 730, 0, -730, -1456, -2174, -2881, -3572, -4245, -4894, -5518, -6112, -6673, -7199, -7686, -8133, -8536, -8893, -9203, -9464, -9675, -9834, -9940, -9993, -9993, -9940, -9834, -9675, -9464, -9203, -8893, -8536, -8133, -7686, -7199, -6673, -6112, -5518, -4894, -4245, -3572, -2881, -2174, -1456, -730};

short tableG[BufferG] ={0, 765, 1526, 2279, 3017, 3738, 4437, 5110, 5753, 6362, 6934, 7466, 7953, 
8394, 8785, 9125, 9411, 9643, 9817, 9934, 9993, 9993, 9934, 9817, 9643, 9411, 9125, 8785, 8394, 
7953, 7466, 6934, 6362, 5753, 5110, 4437, 3738, 3017, 2279, 1526, 765, 0, -765, -1526, -2279, -3017, 
-3738, -4437, -5110, -5753, -6362, -6934, -7466, -7953, -8394, -8785, -9125, -9411, -9643, -9817,
 -9934, -9993, -9993, -9934, -9817, -9643, -9411, -9125, -8785, -8394, -7953, -7466, -6934, -6362, 
 -5753, -5110, -4437, -3738, -3017, -2279, -1526, -765};
 
short tableGs[BufferGs] ={0, 815, 1625, 2424, 3206, 3968, 4703, 5406, 6074, 6701, 7284, 7818, 8301, 8727, 9096, 9405, 9650, 9832, 9948, 9998, 9981, 9898, 9749, 9535, 9258, 8919, 8521, 8066, 7557, 6999, 6393, 5745, 5059, 4339, 3590, 2817, 2026, 1221, 408, -408, -1221, -2026, -2817, -3590, -4339, -5059, -5745, -6393, -6999, -7557, -8066, -8521, -8919, -9258, -9535, -9749, -9898, -9981, -9998, -9948, -9832, -9650, -9405, -9096, -8727, -8301, -7818, -7284, -6701, -6074, -5406, -4703, -3968, -3206, -2424, -1625, -815};
 
short tableA[BufferA] = {0, 860, 1713, 2554, 3375, 4172, 4938, 5667, 6354, 6995, 7583, 8115, 8588,
8996, 9338, 9611, 9813, 9942, 9998, 9979, 9887, 9721, 9484, 9176, 8800, 8359, 7856, 7296, 6681, 6016, 
5307, 4559, 3777, 2967, 2135, 1287, 430, -430, -1287, -2135, -2967, -3777, -4559, -5307, -6016, -6681, 
-7296, -7856, -8359, -8800, -9176, -9484, -9721, -9887, -9979, -9998, -9942, -9813, -9611, -9338, -8996, 
-8588, -8115, -7583, -6995, -6354, -5667, -4938, -4172, -3375, -2554, -1713, -860};

short tableAs[BufferAs] = {0, 909, 1811, 2698, 3562, 4397, 5196, 5951, 6657, 7308, 7899, 8424, 8879, 9260, 9565, 9791, 9935, 9997, 9977, 9873, 9688, 9423, 9079, 8660, 8170, 7611, 6990, 6311, 5579, 4802, 3984, 3133, 2257, 1362, 455, -455, -1362, -2257, -3133, -3984, -4802, -5579, -6311, -6990, -7611, -8170, -8660, -9079, -9423, -9688, -9873, -9977, -9997, -9935, -9791, -9565, -9260, -8879, -8424, -7899, -7308, -6657, -5951, -5196, -4397, -3562, -2698, -1811, -909};

short tableB[BufferB] = {0, 965, 1921, 2859, 3771, 4647, 5480, 6262, 6985, 7643, 8230, 8740, 9168, 9511,
 9764, 9927, 9997, 9974, 9857, 9649, 9350, 8964, 8495, 7946, 7323, 6631, 5878, 5070, 4214, 3319, 2393, 1445,
  483, -483, -1445, -2393, -3319, -4214, -5070, -5878, -6631, -7323, -7946, -8495, -8964, -9350, -9649, -9857,
   -9974, -9997, -9927, -9764, -9511, -9168, -8740, -8230, -7643, -6985, -6262, -5480, -4647, -3771, -2859, -1921, 
   -965};
   
int i = 0;
short gain = 10;                        
short f = 0;

//functions that plays notes, based on data read from an encoded music file. A note(int from 0 to 12) is played
//for a certain duration(int), at a certain octave(int, power of 2), with an optional legato(if tieNote is set to 1)
void playNote(int note, int duration, int octave, int tieNote){
        int skip = 0;
	int thisBuffer = 0;
	
	//notes are played by matching the given note int(in range 0-12) to the musical note that it corresponds, 
	//and then loading the sine table of that note and iterating over that table, element by element. Each element
	//is then sent to the onboard DAC at the given sampling frequency to produce an audible tone. To produce a note
	//that is n octaves higher, simply skip 2^n elements when iterating through the table
	
	//play C note
        if(note == 0 ){
                thisBuffer = BufferC;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableC[skip]);
                }
        }
        
        //play C# note
        else if(note == 1 ){
                thisBuffer = BufferCs;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableCs[skip]);
                }
        }
        
        //play D note
        else if(note == 2 ){
                thisBuffer = BufferD;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableD[skip]);
                }
        }
        
        //play D# note
        else if(note == 3 ){
                thisBuffer = BufferDs;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableDs[skip]);
                }
        }
        
        //play E note
        else if(note == 4 ){
                thisBuffer = BufferE;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableE[skip]);
                }
        }
        
        //play F note
        else if(note == 5 ){
                thisBuffer = BufferF;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableF[skip]);
                }
        }
        
        //play F# note
        else if(note == 6 ){
                thisBuffer = BufferFs;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableFs[skip]);
                }
        }
        
        //play G note
        else if(note == 7 ){
                thisBuffer = BufferG;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableG[skip]);
                }
        }
        
        //play G# note
        else if(note == 8 ){
                thisBuffer = BufferGs;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableGs[skip]);
                }
        }
        
        //play A note
        else if(note == 9 ){
                thisBuffer = BufferA;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableA[skip]);
                }
        }
        
        //play A# note
        else if(note == 10 ){
                thisBuffer = BufferAs;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableAs[skip]);
                }
        }
        
        //play B note
        else if(note == 11 ){
                thisBuffer = BufferB;
                for(i = 0; i < duration; i++ ){
                        skip += octave;
                        skip = skip % thisBuffer;
                        output_sample(gain*tableB[skip]);
                }
        }
        
        //play nothing, equivalent to a rest in musical terms
        else if(note == 12 ){
                for(i = 0; i < duration; i++ ){
                        output_sample(0);
                }
        }
        
        //if tieNote is set to 1, simply return, so that next note played is legato'd with this note
        if(tieNote == 1){
                return;
        }
        //else include a short rest between notes so there is no slurring
        else {
                for(i = 0; i < 100; i++ ){
                      output_sample(0);  
                }
        }      
}


void main()
{
		//char variables used to parse txt file
		char line[1024];
		char *line_tok;
		
		//variables that set the paramters of note to be played
		int txtNote;
		int txtDuration;
		int txtOctave;
		int txtTie;
		
	        int pos = 0;

		comm_poll(); 

                //txt file containing encoded song
		fin = fopen("song.txt", "r");
		
		//parse the txt file with ',' delimiter
		while(fgets(line, sizeof(line), fin) != NULL ){
		
			pos = 0;
			line[strlen(line)-1] = '\0';
			line_tok = (char*)strtok(line, ","); //break up each line into tokens
	
	                //based on position of each token...
			while(line_tok != NULL){
		
		                //convert token to int and set note paramaters accordingly
		                
				if (pos == 0){
					txtNote = atoi(line_tok); //extract the encoded note
					//TODO: ADD CHORD PLAYING STRUCTURE!!!
				}
				if (pos == 1){
					txtDuration = atoi(line_tok); //extract note duration
				}
				if (pos == 2){
					txtOctave = atoi(line_tok); //extract octave at which note is played
				}
				if (pos == 3){
					txtTie = atoi(line_tok); //extract legato option
				}

				pos++;
				line_tok = (char*)strtok(NULL, ",");
				
			}
			//using the paramters extracted from this particular line, 
			//play this note
			playNote(txtNote, txtDuration, txtOctave, txtTie);

		}
		//repeat until end of file, and close said file
		fclose(fin);                                             
}
