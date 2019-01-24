#include "PpmGenerator.h"

PpmGenerator* generator;
int* signals;

void setup()
{ 
  Serial.begin(9600);
  generator = new PpmGenerator(8, 9);
  signals = new int[2];
}

void loop()
{
  signals[0] = map(analogRead(A0), 0, 1023, 1000, 2000);
  signals[1] = map(analogRead(A1), 0, 1023, 1000, 2000);
  generator->assignSignals(signals, 2);
}

ISR( TIMER1_COMPA_vect )
{ 
  generator->run();
}
