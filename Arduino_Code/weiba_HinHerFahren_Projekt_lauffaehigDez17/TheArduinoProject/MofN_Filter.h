#ifndef _MofN_Filter_h_
#define _MofN_Filter_h_

class MofN_Filter
{
   double        n;
   double        outOfM;
   double        summeAbtastwerte;
   unsigned char signalVorher;

public:
   MofN_Filter(double n, double outOfM);
   unsigned char checkNofM(double abtastwert); // liefert 0 oder 1
};

#endif