#include "Wagenabkoppler.h"
#include "Arduino.h"

Wagenabkoppler::Wagenabkoppler(int wagenabkopplerPin)
{
  this->wagenabkopplerPin = wagenabkopplerPin;

//Initialisierung
  pinMode(wagenabkopplerPin,OUTPUT);
  zustand = false;
  digitalWrite(wagenabkopplerPin, zustand);
}

void Wagenabkoppler::hoch()
{
  zustand = 1;
  digitalWrite(wagenabkopplerPin, zustand);
}

void Wagenabkoppler::runter()
{
  zustand = 0;
  digitalWrite(wagenabkopplerPin, zustand);
}

void Wagenabkoppler::switchLage()
{
  zustand = 1 - zustand;
  digitalWrite(wagenabkopplerPin, zustand);
}
