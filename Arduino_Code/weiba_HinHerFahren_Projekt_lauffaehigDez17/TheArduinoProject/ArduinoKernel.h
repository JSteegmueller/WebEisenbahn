#ifndef _ArduinoKernel_h_  // 6. Dez 2017
#define _ArduinoKernel_h_
#include "Arduino.h"        // Prinzipiell .. 
#include <inttypes.h>
#include "ChatterPin.h"     // Wegen TRIGGER_DN, TRIGGER_UP, TRIGGER_ANY

#define MAX_ANZAHL_TIMERS       10
#define MAX_ANZAHL_PINS         14
#define MAX_ANZAHL_ARDUINO_APPS 10
#define MAX_ANZAHL_EVENT_BYTES  2

class ArduinoApp;
class ArduinoTimer;
class ArduinoApp_Heartbeat;
class BitList;
class ArduinoKernel
{
  public:
    ///////////////////////////////////////////////
    // Interrupts
    ///////////////////////////////////////////////
    virtual void appEvent(int idEvent) = 0; // Wird von ArduinoKernel oder anderen Modulen aufgerufen, wenn ein Event passiert
    void stopTimerInterrupt(unsigned char interruptID);  //Timer existiert weiter, ist aber angehalten
    void deleteTimerInterrupt(unsigned char interruptID);  //Timer existiert nicht mehr
    unsigned char setTimerInterrupt(unsigned long int mikroSec, unsigned int nEvents = 0xFF); // Timer stoppt nach n Events; n=0xFF --> Timer stoppt nie. Wenn mikroSek == 0, dann läuft der Timer nach dem erzeugen nicht. Return: EventID
    void restartTimerInterrupt(unsigned char interruptID, unsigned long int mikroSec, unsigned int nEvents = 0xFF); //Startet den Timer erneut, ohne einen neuen Timer zu erzeugen.
    void stopPinInterrupt(unsigned char interruptID);
    unsigned char setPinInterrupt( unsigned char pin,                 // Return: EventID
                                   unsigned char pinmode,             // OUTPOT, INPUT, INPUT_PULLUP
                                   unsigned char trigger_UpDnAny,     // TRIGGER_DN TRIGGER_UP TRIGGER_ANY
                                   bool          useHardwareInterrupt = false,
                                   unsigned long sperrzeitMikroSec    = 5000,
                                   unsigned char n                    = 10,
                                   unsigned char outOfm               = 12);

    ///////////////////////////////////////////////
    // Digital Services
    ///////////////////////////////////////////////
    static void setHalfbyte(unsigned char pins[], unsigned char bitmuster, bool setPinModeOutput=true);  // firstPin geht auf Bit 0
    static void setHalfbyte(unsigned char firstPin, unsigned char bitmuster, bool setPinModeOutput=true);// firstPin+1 auf Bit 1
    static void setByte(unsigned char firstPin, unsigned char bitmuster, bool setPinModeOutput=true);    // ...

    ///////////////////////////////////////////////
    // Signale zum debuggen ausgeben (Alles blocking Calls)
    ///////////////////////////////////////////////
    static void   slowPulse(int pin); // heller-heller-heller-aus
    static void   blinkXTimes(unsigned char pin, unsigned char xTimes); // xTimes mal ein-aus 
    static void   swapPin(unsigned char pin, bool setOutput = true); // ein - aus - ein - aus - ...
    void          setHeartbeatPin(unsigned char pin, bool invert = false);  // Disables Heartbeat if Pin = 0
    void          setError(unsigned char errorID); // Veraendert den Heartbeat: alle HEARTBEATERRORMILLIS blinkt er so oft wie errorID angibt. Siehe Errorcodes.h
    unsigned char getError(); // Liefert den ersten Fehler der auftrat. (Weitere Fehler werden immer verworfen.)

    ///////////////////////////////////////////////
    // Board-ID: Bestimmte Digitalpins auslesen
    ///////////////////////////////////////////////
    static unsigned char getBoardID(unsigned char lsb_pin, unsigned char numberOfPins); // Ab lsb_pin die n�chsten numberOfPins als Bitmuster auslesen
    static unsigned char getBoardID(); // Die zuvor gelesene ID nochmal abrufen aber nicht neu lesen.

    ///////////////////////////////////////////////
    // Event-Handling
    ///////////////////////////////////////////////
    static unsigned char createNewEvent(); // Liefert eine eindeutige Event-ID. Darf vom Anwender aufgerufen werden. Liefert niemals 0 (Null)
    static void deleteEvent(unsigned char e); // Gibt das Event wieder für andere Objekte frei
    void event(int idEvent); // Löst das Event aus. 

    ///////////////////////////////////////////////
    // Apps für dieses Betriebssystem
    ///////////////////////////////////////////////
    void addArduinoApp(ArduinoApp* neueApp); 
    void removeArduinoApp(ArduinoApp* dieApp); 

    ///////////////////////////////////////////////
    // Zeitvergleiche
    ///////////////////////////////////////////////
    static long int remainingMicrosecondsTo(unsigned long int timeMicroSec); // klappt auch bei Ueberlauf !

///////////////////////////////////////////////
// Ab hier keine Befehle mehr fuer den Anwender
///////////////////////////////////////////////
    ArduinoKernel(); // Dieser Konstruktor MUSS in cpp-Datei implementiert werden, sonst Absturz des Compilers!
    virtual ~ArduinoKernel(){};

    // Anbindung: App an Arduino
    void setup();
    void loop();

  private:
    //Scheduler
    void scheduleTimers();
    void schedulePins();

    // Klassenattribute
    ArduinoTimer*        dieArduinoTimer [MAX_ANZAHL_TIMERS];
    ChatterPin*          diePinInterrupts[MAX_ANZAHL_PINS  ];
    ArduinoApp*          dieArduinoApps  [MAX_ANZAHL_ARDUINO_APPS];
    static BitList*      freieEvents; 
    ArduinoApp_Heartbeat* myArduinoApp_Heartbeat;

    // Anbindung: App_-Implamentierung
    virtual void appSetup() = 0; //Erst hier sind Arduino-Befehle moeglich!
    virtual void appLoop()  = 0; //Wird immer aufgerufen, wenn es nichts zu tun gibt. (Arduino_mini_Pro_15MHz: ca. 15000 mal pro sec)

    // Board-ID
    static unsigned char boardID;

    // Fuer die Hardware-Interrupts:
    static ArduinoKernel*     this_ArduinoKernel; // Für Ruecksprung aus statischen Funktionen in Klassenmethoden
    static void          attachInterruptFunctionHandler();
    static unsigned char attachInterruptFunctionHandler_PinInterruptID;
    unsigned char        hardwareInterruptID_Pin2;
    unsigned char        hardwareInterruptID_Pin3;
    int                  findTimer(unsigned char interruptID); // liefert index aus dieArduinoTimer[]
    unsigned char        findIndexOfObject(void** objectList, unsigned char sizeOfObjectList, void* object); // If object == 0 we find the next free index :)
};

#endif

