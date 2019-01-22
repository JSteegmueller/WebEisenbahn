#ifndef _Wagenabkoppler_h_
#define _Wagenabkoppler_h_

class Wagenabkoppler
{
  private:
    int  wagenabkopplerPin;
    bool zustand; //zustand false = Unten, true = Oben

public:
       Wagenabkoppler(int wagenabkopplerPin);
  void hoch();
  void runter();
  void switchLage();
};

#endif
