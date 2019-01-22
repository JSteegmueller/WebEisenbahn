#include "ArduinoApp_Heartbeat.h"
#include "ArduinoKernel.h"
#include "PulsWeitenModulator.h"
#include "Errorcodes.h"

#define DEFAULTHEARTBEATPIN   13
#define HEARTBEATMILLISECONDS 1500

unsigned char ArduinoApp_Heartbeat::pinHeartbeat;

ArduinoApp_Heartbeat::ArduinoApp_Heartbeat(ArduinoKernel* eineAppBase)
  : ArduinoApp(eineAppBase)
{
  theErrorID          = ERRCODE__NO_ERROR;
  derHeartbeatPWM     = new PulsWeitenModulator();
  pinHeartbeat        = DEFAULTHEARTBEATPIN;
  dieHeartbeatTimerID = 0;
  invert              = false;
}

void ArduinoApp_Heartbeat::appSetup()
{
  setHeartbeatPin(pinHeartbeat, invert); // Koennte zwischen Konstruktor und hier geaendert worden sein! Notwendig wegen Timer-Initialisierung
}

void ArduinoApp_Heartbeat::appLoop()
{
}

bool ArduinoApp_Heartbeat::appEvent(int idEvent)
{
  if (idEvent == dieHeartbeatTimerID)
  {
    unsigned char hasChanged;
    unsigned char valueOnOff = derHeartbeatPWM->checkPWM(&hasChanged);
    if (hasChanged == 1)
    {
      eventHeartBeat(valueOnOff);
    }
    return true;
  }

  return false;
}

void ArduinoApp_Heartbeat::setHeartbeatPin(unsigned char pin, bool invert)
{
  this->invert = invert;
  
  if (pin == 0)
  {
    pinHeartbeat = 0;
    if (dieHeartbeatTimerID != 0)
    {
      meinArduinoKernel->deleteTimerInterrupt(dieHeartbeatTimerID);
      dieHeartbeatTimerID = 0;
    }
  }
  else
  {
    pinMode(pin, OUTPUT); // immer setzen!
    pinHeartbeat = pin;
    derHeartbeatPWM->setPWM_OnSteps_AllSteps(1, 10);
    if (dieHeartbeatTimerID == 0)
    {
      dieHeartbeatTimerID = meinArduinoKernel->setTimerInterrupt( (unsigned long)HEARTBEATMILLISECONDS * (unsigned long)100); // *1000 / 10
    }
  }
}

void ArduinoApp_Heartbeat::eventHeartBeat(unsigned char valueLedOn)
{
  if (theErrorID == ERRCODE__NO_ERROR)
  {
    if (invert)
    {
       digitalWrite(pinHeartbeat, 1-valueLedOn);
    }
    else
    {
       digitalWrite(pinHeartbeat, valueLedOn);
    }
  }
  else
  {
    if (valueLedOn == 1)
    {
       // Im Fehlerfall haben wir eine ausreichend langsamere Taktung!
       ArduinoKernel::slowPulse(pinHeartbeat);
       ArduinoKernel::blinkXTimes(pinHeartbeat, theErrorID);
    }
  }
}

void ArduinoApp_Heartbeat::setError(unsigned char errorID)
{
  // Nur wenn nicht schon ein Fehler vorliegt
  if (theErrorID == ERRCODE__NO_ERROR)
  {
    theErrorID = errorID;
  }
}



