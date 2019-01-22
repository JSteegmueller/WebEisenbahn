#include "Lok.h"
#include "Arduino.h"

Lok::Lok(int newGeschwPin, int newFahrtRichtungsPin)
{
  geschwindigkeitsPin = newGeschwPin;
  fahrtRichtungsPin = newFahrtRichtungsPin;
  zustand = 0;

  //Alle Pins auf Output
  pinMode(geschwindigkeitsPin, OUTPUT);
  pinMode(fahrtRichtungsPin, OUTPUT);

  //Pins auf Startwerte
  analogWrite(geschwindigkeitsPin, 180);
  digitalWrite(fahrtRichtungsPin, 0);
}

void Lok::fahrtRichtungVor()
{
  zustand = 0;
  digitalWrite(fahrtRichtungsPin, zustand);
}

void Lok::fahrtRichtungZurueck()
{
  zustand = 1;
  digitalWrite(fahrtRichtungsPin, zustand);
}

void Lok::switchFahrtRichtung()
{
  zustand = 1 - zustand;
  digitalWrite(fahrtRichtungsPin, zustand);
}

//Pulsweitenmodulation setzt Geschwindigkeit
void Lok::setGeschwindigkeit(int newGeschwindigkeit)
{
  //Analoge Werte zwischen 0..255, nicht mehr/weniger
  if( newGeschwindigkeit <= 255 && newGeschwindigkeit >= 0)
  {
    analogWrite(geschwindigkeitsPin, newGeschwindigkeit);
  }
}
