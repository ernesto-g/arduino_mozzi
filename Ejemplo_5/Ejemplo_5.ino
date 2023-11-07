#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/saw2048_int8.h>
#include <ADSR.h>
#include <StateVariable.h>
#include <IntMap.h>
#include <AudioOutput.h>

Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> oscSaw(SAW2048_DATA);
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;
StateVariable <LOWPASS> svf;

float ARP[4]={261.6256,329.6276,391.9954,329.6276}; // C4 E4 G4 E4 
int arpIndex=0;
float centerFreq;
int tick=0;
int timeoutGate=0;
int gateState=0;
IntMap mapResonance(0,1023,255,1); // 0-> 255 (res min) ; 1023 -> 1 (res max)

void setup(){
  startMozzi();
  oscSaw.setFreq(440); // 440Hz
  
  envelope.setADLevels(255,64);
  envelope.setTimes(50,100,100,250);   
}

void updateControl(){
  tick++; // 1 tick:15,625ms
  if(tick==15) // 256bpm=234ms
  {    
       centerFreq = ARP[arpIndex];
       oscSaw.setFreq(centerFreq);
       envelope.noteOn();
       timeoutGate=0;
       gateState=1;
       arpIndex++;
       if(arpIndex>=4) 
        arpIndex=0;
              
       tick=0;     
  }

  if(gateState==1)
  {
      timeoutGate++;
      if(timeoutGate==7) //100ms
      {
          gateState=0;
          envelope.noteOff();
      }
  }

  envelope.update();
  
  svf.setCentreFreq(mozziAnalogRead(A1)*2 + 20); // 20Hz a 2.046Hz
  svf.setResonance(mapResonance(mozziAnalogRead(A0)));
  
}

int updateAudio(){        
  int out =  (int) (envelope.next() * oscSaw.next())>>8;  
  out = svf.next(out);
  //clip
  /* if(out>243)
    out = 243;
  else if(out<-244)
    out = -244; */
  //____  
  out = CLIP_AUDIO(out);
  return out; // must be in the range -244 to 243 in Mozzi’s default STANDARD audio mode.
}

void loop(){
  audioHook(); // required here
}
