#include "ArduinoMain.h"
#include <Arduino.h>
#include "ArduinoAppBahnanlage.h"

// Beim Programmstart vor dem Booten
ArduinoMain::ArduinoMain()
{
  //
  // Hier nichts schreiben!
  //
}  

// Beim Programmstart nach dem Booten
void ArduinoMain::appSetup()
{
  meineBahnanlage = new ArduinoAppBahnanlage(this);
}

// Nach appSetup und dann immer ca 15000 mal pro Sekunde
void ArduinoMain::appLoop()
{
}

// Hier werden Ereignisse bearbeitet
void ArduinoMain::appEvent(int idEvent)
{
}


