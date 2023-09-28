#include <MozziGuts.h>
#include <Oscil.h> 
#include <tables/saw2048_int8.h>
#include <ADSR.h>

Oscil <SAW2048_NUM_CELLS, AUDIO_RATE> oscSaw(SAW2048_DATA);
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;

float ARP[4]={261.6256,329.6276,391.9954,329.6276}; // C4 E4 G4 E4 
int arpIndex=0;
float centerFreq;
int tick=0;
int timeoutGate=0;
int gateState=0;

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
}

int updateAudio(){  
  return (envelope.next() * oscSaw.next())>>8;
}

void loop(){
  audioHook(); // required here
}
