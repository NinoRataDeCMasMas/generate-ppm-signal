#include <Arduino.h>

class PpmGenerator {
  
  int  channelNumber;     // set the number of chanels
  int  defaultServoValue; // set the default servo value
  long ppmFrameLen;       // set the PPM frame length in microseconds (1ms = 1000µs)
  int  ppmPulseLen;       // set the pulse length
  int  onState;           // set polarity of the pulses: 1 is positive, 0 is negative
  int  sigPin;            // set PPM signal output pin on the arduino
  boolean state = true;
  /* this array holds the servo values for the ppm signal change theese values in your code
  (usually servo values move between 1000 and 2000) */
  int* ppm;

public:
  PpmGenerator( int _channelNumber, int _sigPin );
  void assignSignals( int signals[], int signalNumber );
  void run( void );
};
  
