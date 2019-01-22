#include "ChatterPin.h"
#include "Arduino.h"
#include "ArduinoTimer.h"

#ifdef ARDUINO_SIMULATION
   #include "Log.h"
#endif

ChatterPin::ChatterPin( unsigned char pin,
                        unsigned char trigger_UpDnAny,
                        unsigned long sperrzeitMikroSec,
                        unsigned char n,
                        unsigned char outOfm)
:derMofN_Filter(n, outOfm),
 derTimeBlockFilter(sperrzeitMikroSec)
{
   myID = 0;
   pinAndTriggermode = pin + trigger_UpDnAny;
   signalVorher = 255;
}

unsigned char ChatterPin::checkEvent(char value)
{
   unsigned char pinValue;

   if (value == -1)
   {
      pinValue = digitalRead(getPin());
   }
   else
   {
      pinValue = value;
   }

   if (signalVorher == 255) signalVorher = derMofN_Filter.checkNofM(pinValue);

   // Muss regelmaessig aufgerufen werden
   unsigned char gefilterterAbtastwert = derMofN_Filter.checkNofM(pinValue);

   if (signalVorher != gefilterterAbtastwert)
   {
         signalVorher = gefilterterAbtastwert;

         if ((pinAndTriggermode & 0x20) > 1)
         {
            if (derTimeBlockFilter.checkEventAllowed()==true)
            {
               return 1;
            }
         }
         else if (((pinAndTriggermode&0x10)>>4) == gefilterterAbtastwert)
         {
            if (derTimeBlockFilter.checkEventAllowed()==true)
            {
               return 1;
            }
         }
   }

   return 0;
}

unsigned char ChatterPin::getPin()
{
  return pinAndTriggermode & 0x0F;
}

