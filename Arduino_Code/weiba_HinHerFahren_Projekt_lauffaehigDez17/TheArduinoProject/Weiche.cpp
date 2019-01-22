#include "Weiche.h"
#include "Arduino.h"

Weiche::Weiche(int weichenPinRechts, int weichenPinLinks)
{
  this->weichenPinLinks = weichenPinLinks;
  this->weichenPinRechts = weichenPinRechts;
  zustand = 0;

  //Pinmodes setzen
  pinMode(weichenPinLinks, OUTPUT);
  pinMode(weichenPinRechts, OUTPUT);
  digitalWrite(weichenPinLinks, LOW);
  digitalWrite(weichenPinRechts, LOW);
}


void Weiche::setRechts()
{
  zustand = 2;
  digitalWrite(weichenPinRechts, 1);
}

void Weiche::setLinks()
{
  zustand = 1;
  digitalWrite(weichenPinLinks, 1);
}

//Wechselt je nach vorherigem zustand die Richtung
void Weiche::switchRichtung()
{
  if( zustand == 1 || zustand == 0)
  {
    zustand = 2;
    digitalWrite(weichenPinRechts, 1);
  }
  else if( zustand ==2)
  {
    zustand = 1;
    digitalWrite(weichenPinLinks, 1);
  }
}

//Setzt entsprechenden Weichenpin nach 20 milliSec auf 0 zurück
void Weiche::timerEventPulsEnd()
{
  if( zustand == 1)
  {
    digitalWrite(weichenPinLinks, 0);
  }
  else if(zustand == 2)
  {
    digitalWrite(weichenPinRechts, 0);
  }
}
