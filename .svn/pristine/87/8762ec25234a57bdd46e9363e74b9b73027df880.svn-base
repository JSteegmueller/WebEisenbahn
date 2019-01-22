#ifndef _ArduinoApp_h_
#define _ArduinoApp_h_

#include <Arduino.h>
#include "ArduinoKernel.h" // Nuetzlich in abgeleiteten klassen

class ArduinoApp
{
  private:
    unsigned char appEventID;
  protected:
    ArduinoKernel* meinArduinoKernel;

  public:
    ArduinoApp(ArduinoKernel* einArduinoKernel);
    virtual ~ArduinoApp();
    unsigned char getAppEventID();

    // Wird von AppBase aufgerufen
    void setup();
    void loop();
    bool event(int idEvent); // Events aus AppMain, die für mich sind --> return true (= Event wurde verarbeitet)

    virtual void appSetup() {};
    virtual void appLoop () {};
    virtual bool appEvent(int idEvent) {
      return false;
    }; // return false = Event nicht verarbeitet.
};

#endif
