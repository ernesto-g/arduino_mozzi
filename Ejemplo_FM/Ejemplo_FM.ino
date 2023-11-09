#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/sin2048_int8.h> 

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> lfo(SIN2048_DATA);

void setup(){
  startMozzi();
  aSin.setFreq(440); // 440Hz
  lfo.setFreq(8);
}

void updateControl(){

  lfo.setFreq(mozziAnalogRead(A0)/2 + 1); // 1Hz a 512Hz
}

int updateAudio(){

  Q15n16 modIndex = Q0n7_to_Q15n16(lfo.next()*10); // signed fractional number using 15 integer bits and 16 fractional bits, represents -32767.999 to 32767.999
  
  return aSin.phMod(modIndex);
}

void loop(){
  audioHook(); // required here
}
