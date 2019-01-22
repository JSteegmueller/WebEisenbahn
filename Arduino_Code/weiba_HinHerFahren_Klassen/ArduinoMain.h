#ifndef _ArduinoMain_h_
#define _ArduinoMain_h_
#include "ArduinoKernel.h"

class ArduinoAppBahnanlage;

class ArduinoMain : public ArduinoKernel
{
  public:
    ArduinoMain();
    void appLoop();
    void appSetup();
    void appEvent(int idEvent);
    ArduinoAppBahnanlage* meineBahnanlage; 
};

#endif




