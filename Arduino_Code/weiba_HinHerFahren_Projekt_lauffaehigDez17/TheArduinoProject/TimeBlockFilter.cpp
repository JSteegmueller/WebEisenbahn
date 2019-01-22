#include "TimeBlockFilter.h"
#include "ArduinoTimer.h"
#include "ArduinoKernel.h"

TimeBlockFilter::TimeBlockFilter(unsigned long int sperrzeitMikroSec)
{
   this->sperrzeitMikroSec = sperrzeitMikroSec;
   mikroSecLastEvent = 0;
}

bool TimeBlockFilter::checkEventAllowed()
{
   if (ArduinoKernel::remainingMicrosecondsTo(sperrzeitMikroSec + mikroSecLastEvent)<=0)
   {
      mikroSecLastEvent = micros();
      return true;
   }
   else
   {
      return false;
   }
}
