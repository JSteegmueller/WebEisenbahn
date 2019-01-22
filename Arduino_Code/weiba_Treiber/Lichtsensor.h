#ifndef _Lichtsensor_h_
#define _Lichtsensor_h_
#include "Arduino.h"

class Lichtsensor
{
  private:
    uint8_t analogSensorPin;
    double  letzteMesswerte[20];
    int     aktWert = 0;
    double  umgebungslicht;

    double  berechneDurchschnittArray(double zahlenArray[], int arrayGroesse);

  public:
           Lichtsensor(uint8_t pin);
    int    read();
    double readAVG();
    void   setUmgebungslicht(double umgebungslicht);
};

#endif
