#include "Arduino.h"
#include "ArduinoMain.h"
ArduinoMain* dieApp;

void setup()
{
  //Serial.begin(9600);
  //delay(3000);
  //Serial.println("\n---start serial---");
  dieApp = new ArduinoMain();
  dieApp->setup();
}

void loop()
{
  dieApp->loop();
}
