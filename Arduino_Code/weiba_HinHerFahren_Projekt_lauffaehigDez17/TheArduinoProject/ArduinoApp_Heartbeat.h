#ifndef _ArduinoApp_Heartbeat_h_
#define _ArduinoApp_Heartbeat_h_

#include "ArduinoApp.h"

/*
   Lässt die ausgewählte LED alle 1,5 Sekunden aufblitzen. 
   Läuft im Mainloop und zeigt damit an, dass der Controller nicht abgestürzt ist.

   Es kann ein Fehlerzustand eingestellt werden: setError(zahl)
   In diesem Fall erfolgt kein aufblitzen sonder ein Slowpulse, gefolgt von so vielen 
   kurzen Blitzen wie die Variable Zahl angibt.
   Weitere Fehler werden ignoriert. Nur der Erste hat Effekt.

   Wählt man den LED-Pin 0 an: setHeartbeatPin(0), so wird der Heartbeat und auch die 
   Fehlermeldung abgeschaltet.
*/

class PulsWeitenModulator;

class ArduinoApp_Heartbeat : public ArduinoApp
{
   PulsWeitenModulator* derHeartbeatPWM;
   unsigned char        dieHeartbeatTimerID;
   static unsigned char pinHeartbeat;
   unsigned char        theErrorID;
   bool                 invert;

   void                 eventHeartBeat(unsigned char value);

   public:
   ArduinoApp_Heartbeat(ArduinoKernel* eineAppBase);

   virtual void appSetup();
   virtual void appLoop ();
   virtual bool appEvent(int idEvent);

   void setHeartbeatPin(unsigned char pin, bool invert);
   unsigned char getError(){return theErrorID;}; // Liefert den ersten Fehler der auftrat. (Weitere Fehler werden immer verworfen.)
   void setError(unsigned char errorID);
};

#endif
