#ifndef _Weiche_h
#define _Weiche_h

class Weiche
{
  private:
    //Aus Sicht der Platine
    int weichenPinRechts;
    int weichenPinLinks;
    int zustand;          //1 = links, 2 = rechts

  public:
         Weiche(int weichenPinRechts, int weichenPinLinks);
    void setRechts();
    void setLinks();
    void switchRichtung();
    void timerEventPulsEnd();     //Weiche braucht 20 milliSec-Puls
};

#endif
