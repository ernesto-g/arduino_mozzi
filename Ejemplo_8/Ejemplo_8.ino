#include <MozziGuts.h>
#include <Oscil.h> 
#include <samples/abomb16384_int8.h>
#include <Sample.h>
#include <AudioOutput.h>

Sample <ABOMB_NUM_CELLS, AUDIO_RATE> aSample(ABOMB_DATA);
int tick=0;


void setup(){
  startMozzi();
  aSample.setFreq(1); 
}

void updateControl(){
  tick++; // 1 tick:15,625ms
  if(tick==60) // 937,5ms
  {    
       aSample.start();              
       tick=0;     
  }
}

int updateAudio(){          
  int out =  (int) aSample.next();  
  
  out = CLIP_AUDIO(out);
  
  return  out;
}

void loop(){
  audioHook(); // required here
}
