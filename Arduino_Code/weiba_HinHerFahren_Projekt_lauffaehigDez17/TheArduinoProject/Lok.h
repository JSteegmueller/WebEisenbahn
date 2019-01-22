#ifndef _Lok_h_
#define _Lok_h_

class Lok
{
  private:
    int  geschwindigkeitsPin;
    int  fahrtRichtungsPin;
    bool zustand;     //Richtung 0 (vorwärts) oder 1 (rückwärts)

  public:
         Lok(int newGeschwPin, int newFahrtRichtungsPin);
    void fahrtRichtungVor();
    void fahrtRichtungZurueck();
    void setGeschwindigkeit(int newGeschwindigkeit); //Analoge Werte von 0..255 (Normal 170)
    void switchFahrtRichtung();
};

#endif
