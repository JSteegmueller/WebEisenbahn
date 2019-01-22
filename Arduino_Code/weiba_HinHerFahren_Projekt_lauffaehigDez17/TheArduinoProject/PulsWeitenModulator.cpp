#include "PulsWeitenModulator.h"

PulsWeitenModulator::PulsWeitenModulator()
{
  onPWMSteps        = 1;
  numberOfPWMSteps  = 200;
  indexPWMStep      = 0;
  lastReply         = 1; // so the first reply value (thas is zero) signs "hasChanged".
}

void PulsWeitenModulator::setPWM_OnSteps_AllSteps(unsigned char onPWMSteps, unsigned char numberOfPWMSteps)
{
  this->onPWMSteps       = onPWMSteps;
  this->numberOfPWMSteps = numberOfPWMSteps;
  this->indexPWMStep = 0;
}

void PulsWeitenModulator::nextStep()
{
  indexPWMStep++;
  if (indexPWMStep >= numberOfPWMSteps)
  {
    indexPWMStep = 0;
  }
}

unsigned char PulsWeitenModulator::checkPWM(unsigned char* hasChanged)
{
  unsigned char actualReply = (indexPWMStep < onPWMSteps); 
  nextStep();
  if (hasChanged != 0)
  {
    *hasChanged = (lastReply != actualReply);
  }
  
  lastReply = actualReply;
  return actualReply;
}

