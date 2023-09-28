#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/saw2048_int8.h>

Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> oscSaw(SAW2048_DATA);

float ARP[4]={261.6256,329.6276,391.9954,329.6276}; // C4 E4 G4 E4 
int arpIndex=0;
float centerFreq;
int tick=0;

void setup(){
  startMozzi();
  oscSaw.setFreq(440); // 440Hz
}

void updateControl(){
  tick++; // 1 tick:15,625ms
  if(tick==15) // 256bpm=234ms
  {    
       centerFreq = ARP[arpIndex];
       oscSaw.setFreq(centerFreq);
       arpIndex++;
       if(arpIndex>=4) 
        arpIndex=0;
              
       tick=0;     
  }
}

int updateAudio(){
  return oscSaw.next();
}

void loop(){
  audioHook(); // required here
}
