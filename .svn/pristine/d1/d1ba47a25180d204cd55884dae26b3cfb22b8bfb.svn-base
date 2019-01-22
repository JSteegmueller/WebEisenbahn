#include "Lichtsensor.h"


Lichtsensor::Lichtsensor(uint8_t analogPin)
{
  analogSensorPin = analogPin;
  pinMode(analogPin, INPUT);
}

int Lichtsensor::read()
{
  return analogRead(analogSensorPin);
}

double Lichtsensor::readAVG()
{
  letzteMesswerte[aktWert] = analogRead(analogSensorPin);
  aktWert++;

  if ( aktWert >= 20 )
  {
    aktWert = 0;
  }
  return berechneDurchschnittArray(letzteMesswerte, 20);
}

double Lichtsensor::berechneDurchschnittArray(double zahlenArray[], int arrayGroesse)
{
  double summAlleZahlen = 0;
  for ( int x = 0 ; x < arrayGroesse ; x++ )
  {
    summAlleZahlen = summAlleZahlen + zahlenArray[x];
  }
  return summAlleZahlen / arrayGroesse;
}

void Lichtsensor::setUmgebungslicht(double umgebungslicht)
{
  this->umgebungslicht = umgebungslicht;
}
