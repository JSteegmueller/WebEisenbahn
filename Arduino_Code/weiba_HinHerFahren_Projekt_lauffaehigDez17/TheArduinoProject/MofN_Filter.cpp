#include "MofN_Filter.h"

#ifdef ARDUINO_SIMULATION
   #include "Log.h"
#endif

MofN_Filter::MofN_Filter(double n, double outOfM)
{
   this->n = n;
   this->outOfM = outOfM;
   summeAbtastwerte  = outOfM * 0.5;
   signalVorher      = 255;
}

unsigned char MofN_Filter::checkNofM(double abtastwert)
{
//   Log::out(S_+"Abtastwert="+abtastwert);
   if (outOfM < 2) return abtastwert;

   summeAbtastwerte = summeAbtastwerte * (outOfM-1) / outOfM + abtastwert;

   if (signalVorher == 255 ) signalVorher = abtastwert;
   if (signalVorher == 0)
   {
      if (summeAbtastwerte > n)
      {
         signalVorher = 1;
         return 1;
      }
   }
   else  // signalVorher == 1
   {
      if (summeAbtastwerte < (outOfM-n))
      {
         signalVorher = 0;
         return 0;
      }
   }

   return signalVorher;
}

