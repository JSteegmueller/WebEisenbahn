#include "ArduinoApp.h"
#include "ArduinoKernel.h"

ArduinoApp::ArduinoApp(ArduinoKernel* einArduinoKernel)
{
  meinArduinoKernel = einArduinoKernel;
  appEventID = 0;
  meinArduinoKernel->addArduinoApp(this);
}

ArduinoApp::~ArduinoApp()
{
  meinArduinoKernel->removeArduinoApp(this);
  if (appEventID != 0) ArduinoKernel::deleteEvent(appEventID);
}


void ArduinoApp::setup()
{
  appSetup();
};

void ArduinoApp::loop()
{
  appLoop();
};

// Events aus AppMain, die für mich sind --> return true (= Event wurde verarbeitet)
bool ArduinoApp::event(int idEvent)
{
  return appEvent(idEvent);
};

unsigned char ArduinoApp::getAppEventID() // Die appEventID wird nur erzeugt, wenn sie jemand wissen will!
{
  if (appEventID == 0) appEventID = ArduinoKernel::createNewEvent();
  return appEventID;
};

