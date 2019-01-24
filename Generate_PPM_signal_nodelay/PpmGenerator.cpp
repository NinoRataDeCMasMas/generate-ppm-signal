#include "PpmGenerator.h"

PpmGenerator::PpmGenerator( int _channelNumber, int _sigPin )
{
  channelNumber = _channelNumber;
  sigPin = _sigPin;    

  defaultServoValue = 1500;
  ppmFrameLen = 22500;
  ppmPulseLen = 300;
  onState = 1;  
  state = true;

  for(int i = 0; i < channelNumber; ++i) ppm[i] = defaultServoValue;
  
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0;  // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100; // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);   // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void PpmGenerator::assignSignals( int signals[], int signalNumber )
{
  for(int i = 0; i < signalNumber; ++i)
  {
    ppm[i] = signals[i];
  } 
}

void PpmGenerator::run( void )
{
  TCNT1 = 0;
  
  if(state)
  { 
    //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = ppmPulseLen * 2;
    state = false;
  }
  else
  { 
    //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= channelNumber)
    {
      cur_chan_numb = 0;
      calc_rest = calc_rest + ppmPulseLen;// 
      OCR1A = (ppmFrameLen - calc_rest) * 2;
      calc_rest = 0;
    }
    else
    {
      OCR1A = (ppm[cur_chan_numb] - ppmPulseLen) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
