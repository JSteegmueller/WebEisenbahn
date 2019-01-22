#include "Laser.h"
#include "Arduino.h"

Laser::Laser(int pin)
{
  laserPin = pin;
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, 0);
  zustand = 1;
}

void Laser::an()
{
  zustand = 0;
  digitalWrite(laserPin, zustand);
}

void Laser::aus()
{
  zustand = 1;
  digitalWrite(laserPin, zustand);
}

void Laser::laserSwitch()
{
  zustand = 1 - zustand;
  digitalWrite(laserPin, zustand);
}
