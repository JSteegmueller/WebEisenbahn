#ifndef _ArduinoTimer_h_
#define _ArduinoTimer_h_

#define ARDUINOTIMER_MINIMUM_TIMER_INTERVALL_MICRO_SEC 20
#define ARDUINOTIMER_ENDLESS                           0xFF


class ArduinoTimer
{
    unsigned     char myEventID;
    unsigned      int eventsRemaining;  // Der Timer zaehlt bei jedem erfolgreichen checkSwapChange() diese Wert zurueck bis auf 0 (=Stop); Kann den wert "ENDLESS" haben ... laufen bis Stop-Kommando
    unsigned long int intervalTimeMicroSec;
    unsigned long int nextTime;
             long int deltaToNextEventTime;
    unsigned long int ueberschritteneIntervalle;

  public:

    ArduinoTimer();
    ~ArduinoTimer();

    // ID ist nur für den Anwender, die ID hat 
    // keinerlei Einfluss auf das Timer-Vrhalten.
    unsigned char getEventID() {return myEventID;};

    void setIntervalTimeMicroSec(unsigned long int intervalTimeMicroSec, unsigned int nTimes = ARDUINOTIMER_ENDLESS); 
       // Wenn intervalTimeMicroSec < ARDUINOTIMER_MINIMUM_TIMER_INTERVALL_MICRO_SEC dann wird intervalTimeMicroSec auf ARDUINOTIMER_MINIMUM_TIMER_INTERVALL_MICRO_SEC gesetzt.
       // swapsRemaining wird beim Start auf nTimes gesetzt und dann bei jedem Timer-Event verringert, sofern nicht ARDUINOTIMER_ENDLESS.
       // checkSwapChange() liefert immer 0, wenn swapsRemaining = 0. (=stop).
       // Ist nTimes=0, dann wird der Timer in gestopptem Zustand ins Leben gerufen.

    unsigned long int getIntervalTimeMicroSec(){return intervalTimeMicroSec;};

    unsigned long int pollTimerEvent(); // Liefert Aanzahl der überschrittenen Perioden, wenn mindestens 
       // die erste (nächste) Periode zu ende ist. Andernfalls 0. 
       // Wenn AanzahlUeberschrittenePerioden > 1 is, dann wurde zu langsam gepollt!
       
    int getEventsRemaining() {return eventsRemaining;};
    void stop(){eventsRemaining = 0;};
    void start(unsigned int nTimes = ARDUINOTIMER_ENDLESS){setIntervalTimeMicroSec(intervalTimeMicroSec, nTimes);};
};

#endif



