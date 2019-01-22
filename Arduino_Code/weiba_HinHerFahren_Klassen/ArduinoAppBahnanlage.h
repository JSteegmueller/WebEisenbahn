#ifndef _ArduinoAppBahnanlage_h_
#define _ArduinoAppBahnanlage_h

class ArduinoMain;
class Lok;
class Weiche;
class Wagenabkoppler;
class Lichtsensor;
class Laser;

#include "ArduinoApp.h"

class ArduinoAppBahnanlage : public ArduinoApp {

private:
  //Baisc Objekte
  Lok*             meineLok;
  Weiche*          meineWeiche;
  Wagenabkoppler*  meineWagenabkoppler[2];
  Lichtsensor*     meineLichtsensoren[5];
  Laser*           meineLaser;

  //Timer-Geschichten
  int              baseTimer;
  int              laserCheckTimer;
  int              warteTimer;
  int              weichenTimer;
  int              debugTimer;
  bool             mussWarten;
  bool             weichenImpuls;

  //Laser-Messungen
  int              messwerte[5];

public:
  ArduinoAppBahnanlage(ArduinoMain* eineAppBase);
  void appSetup();
  void appLoop();
  bool appEvent(int idEvent);

};

#endif
