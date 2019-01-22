#ifndef _PulsWeitenModulator_h_
#define _PulsWeitenModulator_h_

class PulsWeitenModulator
{
    unsigned char onPWMSteps;
    unsigned char numberOfPWMSteps;
    unsigned char indexPWMStep;
    unsigned char lastReply;

    void nextStep();

  public:
    PulsWeitenModulator();

    void setPWM_OnSteps_AllSteps(unsigned char onPWMSteps, unsigned char numberOfPWMSteps); // Beispiel: 2,10 bedeutet: Von insgesamt 10 Schritten sind die ersten 2 auf 1, die restlichen 8 auf 0.
    void reset() {indexPWMStep = 0;};
    unsigned char checkPWM(unsigned char* hasChanged = 0); // Returns 1 if in High-Range of the pulse, else 0. Steps to the next pulse. " *hasChanged " is 1 if the last reply is different from the actual reply. 0 otherwise
    unsigned char getIndexPWMStep() {return indexPWMStep;};
};

#endif



