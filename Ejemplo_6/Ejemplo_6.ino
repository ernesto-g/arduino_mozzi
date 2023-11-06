#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/triangle2048_int8.h>
#include <ADSR.h>
#include <WaveFolder.h>

Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> oscTri(TRIANGLE2048_DATA);
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;
WaveFolder<> wf;

float ARP[4]={261.6256,329.6276,391.9954,329.6276}; // C4 E4 G4 E4 
int arpIndex=0;
float centerFreq;
int tick=0;
int timeoutGate=0;
int gateState=0;

void setup(){
  startMozzi();
  oscTri.setFreq(440); // 440Hz
  
  envelope.setADLevels(255,64);
  envelope.setTimes(50,100,100,250); 
  
}

void updateControl(){
  tick++; // 1 tick:15,625ms
  if(tick==15) // 256bpm=234ms
  {    
       centerFreq = ARP[arpIndex];
       oscTri.setFreq(centerFreq);
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
  
  int lim = mozziAnalogRead(A0)/4 + 2;
  wf.setLimits(-1*lim, lim);
}

int updateAudio(){        
  int out =  (int) (envelope.next() * oscTri.next())>>8;  
  out = wf.next(out);
  //clip
  if(out>243)
    out = 243;
  else if(out<-244)
    out = -244;
  //____  
  return out; // must be in the range -244 to 243 in Mozzi’s default STANDARD audio mode.
}

void loop(){
  audioHook(); // required here
}
