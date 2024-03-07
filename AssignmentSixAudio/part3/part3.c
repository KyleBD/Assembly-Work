#include <math.h>

#define BUF_SIZE 6100 // about 10 seconds of buffer (@ 8K samples/sec)
#define BUF_THRESHOLD 96 // 75% of 128 word buffer
#define sampleRate 8000
#define NUM_SWITCHES 10

struct audio_t {
	volatile unsigned int control;
	volatile unsigned char rarc;
	volatile unsigned char ralc;
	volatile unsigned char warc;
	volatile unsigned char walc;
    volatile unsigned int ldata;
	volatile unsigned int rdata;
};

struct audio_t *const audiop = ((struct audio_t *)0xff203040);
volatile int *Switches = 0xFF200040;
volatile int *LEDs = 0xff200000;


void initalizeSquareWave(int* squarewave, int frequency){
	int high = 0xFFFFFF;
	int low = 0x0;
	int data = high;
	int halfPeriod = sampleRate / (2 * frequency);


	for (int i = 0; i < BUF_SIZE; ++i) {
		squarewave[i] = data;
		 if ((i + 1) % halfPeriod == 0){
			 data = (data == high) ? low : high;
		 }
	}

}
	
void 
audio_playback(int *samples) {
            int buffer_index = 0;

            audiop->control = 0x8; // clear the output FIFOs
            audiop->control = 0x0; // resume output conversion
            while (buffer_index < BUF_SIZE) {
              // output data if there is space in the output FIFOs
              if (audiop->warc) {
                  audiop->ldata = samples[buffer_index];
                  audiop->rdata = samples[buffer_index];
                  ++buffer_index;
              }
              
             }
}	
//Increase or decrease the wave by factors of 2
/* SW LAYOUT
SW 0-4 INCREASE
SW 5-9 DECREASE
*/
void check_input(int *wave, int *frequency){
    int waveInput = *Switches;

    if(waveInput == 0x1){
        (*frequency) = (*frequency) - 1;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x2){
        (*frequency) = (*frequency) - 4;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x4){
        (*frequency) = (*frequency) - 16;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x8){
        (*frequency) = (*frequency) - 64;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x10){
        (*frequency) = (*frequency) - 128;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x20){
        (*frequency) = (*frequency) + 1;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x40){
        (*frequency) = (*frequency) + 4;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x80){
        (*frequency) = (*frequency) + 16;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x100){
        (*frequency) = (*frequency) + 64;
        initalizeSquareWave(wave, (*frequency));
    }
    else if(waveInput == 0x200){
        (*frequency) = (*frequency) + 128;
        initalizeSquareWave(wave, (*frequency));
    }
    else{
        return;
    }

}


int main(void) {
    int squarewave[BUF_SIZE]; // Buffer to hold the square wave
    int baseLineFrequency = 2000;
    int *curfrequency = &baseLineFrequency;
	initalizeSquareWave(squarewave, *curfrequency); // Generate square wave with frequency 440 Hz
    // Generate square wave
	while(1){  
        check_input(squarewave,curfrequency);
        // Playback the square wave
        audio_playback(squarewave);
	}
    return 0;
}