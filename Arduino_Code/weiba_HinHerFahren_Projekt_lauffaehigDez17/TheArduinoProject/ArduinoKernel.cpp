#include "ArduinoKernel.h"  // Version 4 (Mai 2017)
#include <Arduino.h>
#include "ArduinoTimer.h"
#include "Errorcodes.h"
#include "ChatterPin.h"
#include "ArduinoApp.h"
#include "ArduinoApp_Heartbeat.h"
#include "BitList.h"

unsigned char swapPinOn[MAX_ANZAHL_PINS];

unsigned char ArduinoKernel::boardID;
BitList*      ArduinoKernel::freieEvents;
ArduinoKernel*     ArduinoKernel::this_ArduinoKernel;
unsigned char ArduinoKernel::attachInterruptFunctionHandler_PinInterruptID;

void ArduinoKernel::attachInterruptFunctionHandler()
{
  this_ArduinoKernel->event(attachInterruptFunctionHandler_PinInterruptID);
}

ArduinoKernel::ArduinoKernel()
{
  this_ArduinoKernel            = this;
  boardID                  = 0;
  myArduinoApp_Heartbeat    = 0;
  hardwareInterruptID_Pin2 = 0;
  hardwareInterruptID_Pin3 = 0;

  for (unsigned char i = 0; i < MAX_ANZAHL_TIMERS ; i++)
  {
    dieArduinoTimer[i] = 0;
  }

  for (unsigned char i = 0; i < MAX_ANZAHL_PINS ; i++)
  {
    diePinInterrupts[i] = 0;
  }

  for (unsigned char i = 0; i < MAX_ANZAHL_ARDUINO_APPS ; i++)
  {
    dieArduinoApps[i] = 0;
  }

  for (int i = 0; i < MAX_ANZAHL_PINS; i++)
  {
    swapPinOn[i] = 0;
  }

  freieEvents = new BitList(MAX_ANZAHL_EVENT_BYTES);
}

void ArduinoKernel::setup()
{
  myArduinoApp_Heartbeat = new ArduinoApp_Heartbeat(this);

  appSetup(); // Hier können noch weitere Apps angemeldet werden

  for (unsigned char i = 0; i < MAX_ANZAHL_ARDUINO_APPS; i++)
  {
    if ( dieArduinoApps[i] != 0) dieArduinoApps[i]->setup();
  }
}

void ArduinoKernel::loop()  // Scheduling: Pins und Timers haben Vorrang!
{
  for (unsigned char i = 0; i < MAX_ANZAHL_ARDUINO_APPS; i++)
  {
    schedulePins();
    scheduleTimers();
    if ( dieArduinoApps[i] != 0) dieArduinoApps[i]->loop();
  }

  schedulePins();
  scheduleTimers();

  appLoop();
}

void ArduinoKernel::schedulePins()
{
  for (unsigned char i = 0; i < MAX_ANZAHL_PINS; i++)
  {
    //    Log::out(S_+"i:"+i);
    if ( diePinInterrupts[i] != 0)
    {
      if ( diePinInterrupts[i]->checkEvent() )
      {
        event( diePinInterrupts[i]->getID() );
      }
    }
  }
}

void ArduinoKernel::scheduleTimers()
{
  for (unsigned char i = 0; i < MAX_ANZAHL_TIMERS; i++)
  {
    if ( dieArduinoTimer[i] != 0)
    {
      if (dieArduinoTimer[i]->pollTimerEvent() > 0)
      {
        event(dieArduinoTimer[i]->getEventID());
      }
    }
  }
}

void ArduinoKernel::slowPulse(int pin)
{
  pinMode(pin, OUTPUT);
  int m = 50;
  for (int i = 1; i < m; i++)
  {
    for (int q = 0; q < 10; q++)
    {
      digitalWrite(pin, 1);
      delayMicroseconds(i * i);
      digitalWrite(pin, 0);
      delayMicroseconds(m * m - i * i);
    }
  }
  digitalWrite(pin, 0);
}

unsigned char ArduinoKernel::getBoardID()
{
  return boardID;
}

unsigned char ArduinoKernel::getBoardID(unsigned char lsb_pin, unsigned char numberOfPins)
{
  boardID = 0;
  while (numberOfPins > 0)
  {
    numberOfPins--;
    pinMode(lsb_pin + numberOfPins, INPUT_PULLUP);
    boardID |= (digitalRead(lsb_pin + numberOfPins) << numberOfPins);
  }
  return boardID;
}

void ArduinoKernel::blinkXTimes(unsigned char pin, unsigned char xTimes)
{
  unsigned int  zykluszeit = 400; // darunter für Auge schwer lesbar
  unsigned char einzeit = 40;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  delay(zykluszeit);
  while (xTimes > 0)
  {
    digitalWrite(pin, 1);
    delay(einzeit);
    digitalWrite(pin, 0);
    delay(zykluszeit - einzeit);
    xTimes--;
  }
}

void ArduinoKernel::event(int idEvent)
{
  for (unsigned char i = 0; i < MAX_ANZAHL_ARDUINO_APPS; i++)
  {
    if ( dieArduinoApps[i] != 0)
    {
      if (dieArduinoApps[i]->event(idEvent) == true) return; // Event wurde verarbeitet
    }
  }

  appEvent(idEvent);
}

void ArduinoKernel::setError(unsigned char errorID)
{
  myArduinoApp_Heartbeat->setError(errorID);
}

void ArduinoKernel::setByte(unsigned char firstPin, unsigned char bitmuster, bool setPinModeOutput)
{
  setHalfbyte(firstPin,   bitmuster,    setPinModeOutput);
  setHalfbyte(firstPin + 4, bitmuster >> 4, setPinModeOutput);
}

void ArduinoKernel::setHalfbyte(unsigned char firstPin, unsigned char bitmuster, bool setPinModeOutput)
{
  unsigned char i;
  if (setPinModeOutput == true)
  {
    for (i = firstPin; i < firstPin + 4; i++)
    {
      pinMode(i, OUTPUT);
    }
  }

  unsigned char maske = 1;
  for (i = firstPin; i < firstPin + 4; i++)
  {
    digitalWrite(i, (bitmuster & maske) >> (i - firstPin));
    maske = maske << 1;
  }
}

void ArduinoKernel::setHeartbeatPin(unsigned char pin, bool invert)
{
  myArduinoApp_Heartbeat->setHeartbeatPin(pin, invert);
}

unsigned char ArduinoKernel::setPinInterrupt( unsigned char pin,
    unsigned char pinmode,
    unsigned char trigger_UpDnAny,
    bool          useHardwareInterrupt ,
    unsigned long sperrzeitMikroSec    ,
    unsigned char n                    ,
    unsigned char outOfm               )
{
  pinMode(pin, pinmode);

  if (useHardwareInterrupt == true)
  {
    // Auf Arduino-Werte anpassen
    if (trigger_UpDnAny == TRIGGER_UP)  trigger_UpDnAny = RISING;
    if (trigger_UpDnAny == TRIGGER_DN)  trigger_UpDnAny = FALLING;
    if (trigger_UpDnAny == TRIGGER_ANY) trigger_UpDnAny = CHANGE;

    if (pin == 2)
    {
      hardwareInterruptID_Pin2 = createNewEvent();
      attachInterruptFunctionHandler_PinInterruptID = hardwareInterruptID_Pin2;
      attachInterrupt(0, attachInterruptFunctionHandler, trigger_UpDnAny);
      return hardwareInterruptID_Pin2;
    }
    else if (pin == 3)
    {
      hardwareInterruptID_Pin3 = createNewEvent();
      attachInterruptFunctionHandler_PinInterruptID = hardwareInterruptID_Pin3;
      attachInterrupt(1, attachInterruptFunctionHandler, trigger_UpDnAny);
      return hardwareInterruptID_Pin3;
    }
    else
    {
      setError(ERRCODE__APPBASE_SETPININTERRUPT_PIN_NOT_ALLOWED);
      return 0;
    }

  }
  else
  {
    unsigned char gesuchterIndex = findIndexOfObject((void**)diePinInterrupts, MAX_ANZAHL_PINS, 0); // 0 means: Find an emty place
    if (gesuchterIndex == MAX_ANZAHL_PINS) return 0;

    diePinInterrupts[gesuchterIndex]
      = new ChatterPin(pin,
                       trigger_UpDnAny,
                       sperrzeitMikroSec,
                       n,
                       outOfm);
    diePinInterrupts[gesuchterIndex]->setID(createNewEvent());
    return diePinInterrupts[gesuchterIndex]->getID();
  }
}

void ArduinoKernel::stopPinInterrupt(unsigned char interruptID)
{
  if (interruptID == hardwareInterruptID_Pin2)
  {
    detachInterrupt(0);  // Interrupt fuer Pin 2
    hardwareInterruptID_Pin2 = 0;
  }
  else if (interruptID == hardwareInterruptID_Pin3)
  {
    detachInterrupt(1);  // Interrupt fuer Pin 3
    hardwareInterruptID_Pin3 = 0;
  }
  else
  {
    unsigned char gesuchterIndex;
    for (gesuchterIndex = 0; gesuchterIndex < MAX_ANZAHL_PINS; gesuchterIndex++)
    {
      if (diePinInterrupts[gesuchterIndex]->getID() == interruptID) break;
    }

    if (gesuchterIndex == MAX_ANZAHL_PINS) return;

    delete(diePinInterrupts[gesuchterIndex]);
    diePinInterrupts[gesuchterIndex] = 0;
  }
}

void ArduinoKernel::swapPin(unsigned char pin, bool setOutput)
{
  if (setOutput)
  {
    pinMode(pin, OUTPUT);
  }

  digitalWrite(pin, swapPinOn[pin]);
  swapPinOn[pin] = 1 - swapPinOn[pin];
}

void ArduinoKernel::setHalfbyte(unsigned char pins[], unsigned char bitmuster, bool setPinModeOutput)
{
  unsigned char i;
  unsigned char maske = 1;
  for (i = 0; i < 4; i++)
  {
    if (setPinModeOutput == true)
    {
      pinMode(pins[i], OUTPUT);
    }
    digitalWrite(pins[i], (bitmuster & maske) >> i);
    maske = maske << 1;
  }
}

void ArduinoKernel::restartTimerInterrupt(unsigned char interruptID, unsigned long int mikroSec, unsigned int nEvents)
{
  int index = findTimer(interruptID);
  if (index == -1)
  {
    setError(ERRCODE__APPBASE_EVENT_ID_DOES_NOT_EXIST);
    return;
  }

  dieArduinoTimer[index]->setIntervalTimeMicroSec(mikroSec, nEvents);
}

void ArduinoKernel::stopTimerInterrupt(unsigned char interruptID)
{
  int index = findTimer(interruptID);
  if (index == -1)
  {
    setError(ERRCODE__APPBASE_EVENT_ID_DOES_NOT_EXIST);
    return;
  }

  dieArduinoTimer[index]->stop();
}

void ArduinoKernel::removeArduinoApp(ArduinoApp* dieApp)
{
  unsigned char gesuchterIndex = findIndexOfObject((void**)dieArduinoApps, MAX_ANZAHL_ARDUINO_APPS, dieApp);
  if (gesuchterIndex == MAX_ANZAHL_ARDUINO_APPS) return;
  dieArduinoApps[gesuchterIndex] = 0;
}

void ArduinoKernel::addArduinoApp(ArduinoApp* neueApp)
{
  unsigned char gesuchterIndex = findIndexOfObject((void**)dieArduinoApps, MAX_ANZAHL_ARDUINO_APPS, 0); // 0 means find an emty place
  if (gesuchterIndex == MAX_ANZAHL_ARDUINO_APPS)
  {
    setError(ERRCODE__APPBASE_LIST_OVERFLOW);
    return;
  }

  dieArduinoApps[gesuchterIndex] = neueApp;
}

unsigned char ArduinoKernel::getError()
{
  return myArduinoApp_Heartbeat->getError();
}

void ArduinoKernel::deleteEvent(unsigned char e)
{
  if ( freieEvents->setBit(e, 0) == false )
  {
    this_ArduinoKernel->setError(ERRCODE__APPBASE_EVENT_ID_DOES_NOT_EXIST);
  }
}

unsigned char ArduinoKernel::createNewEvent()
{
  unsigned char event;
  for (event = 1; event < MAX_ANZAHL_EVENT_BYTES * 8; event++) // Niemals Event NULL vergeben!
  {
    if (freieEvents->getBit(event) == 0 )
    {
      freieEvents->setBit(event, 1);
      break;
    }
  }

  if (event == MAX_ANZAHL_EVENT_BYTES * 8)
  {
    this_ArduinoKernel->setError(ERRCODE__APPBASE_LIST_OVERFLOW);
    return 0;
  }
  else
  {
    return event;
  }
}

unsigned char ArduinoKernel::setTimerInterrupt(unsigned long int mikroSec, unsigned int nTimes)
{
  unsigned char gesuchterIndex = findIndexOfObject((void**)dieArduinoTimer, MAX_ANZAHL_TIMERS, 0); // 0 means find an emty place
  if (gesuchterIndex == MAX_ANZAHL_TIMERS)
  {
    setError(ERRCODE__APPBASE_SET_NEW_TIMER_FAILED);
    return 0;
  }

  dieArduinoTimer[gesuchterIndex] = new ArduinoTimer();
  dieArduinoTimer[gesuchterIndex]->setIntervalTimeMicroSec(mikroSec, nTimes);
  return dieArduinoTimer[gesuchterIndex]->getEventID();
}

unsigned char ArduinoKernel::findIndexOfObject(void** objectList, unsigned char sizeOfObjectList, void* object)
{
  unsigned char gesuchterIndex;
  for (gesuchterIndex = 0; gesuchterIndex < sizeOfObjectList; gesuchterIndex++)
  {
    if (objectList[gesuchterIndex] == object) break;
  }
  return gesuchterIndex;
}

void ArduinoKernel::deleteTimerInterrupt(unsigned char interruptID)
{
  int index = findTimer(interruptID);
  if (index == -1) setError(ERRCODE__APPBASE_EVENT_ID_DOES_NOT_EXIST);
  delete(dieArduinoTimer[index]); dieArduinoTimer[index] = 0;
  deleteEvent(interruptID);
}

int ArduinoKernel::findTimer(unsigned char interruptID) // Liefert index
{
  unsigned char gesuchterIndex;
  for (gesuchterIndex = 0; gesuchterIndex < MAX_ANZAHL_TIMERS; gesuchterIndex++)
  {
    if (dieArduinoTimer[gesuchterIndex] == 0) continue;
    if (dieArduinoTimer[gesuchterIndex]->getEventID() == interruptID)
    {
      break;
    }
  }

  if (gesuchterIndex == MAX_ANZAHL_TIMERS) return -1;
  return gesuchterIndex;
}

//static//
long int ArduinoKernel::remainingMicrosecondsTo(unsigned long int timeMicroSec)
{
  return timeMicroSec - micros(); // klappt auch bei Ueberlauf !  Siehe .\Timerueberlauf.txt
}

