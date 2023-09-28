#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h> 

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

void setup(){
  startMozzi();
  aSin.setFreq(440); // 440Hz
}

void updateControl(){
}

int updateAudio(){
  return aSin.next();
}

void loop(){
  audioHook(); // required here
}
