#ifndef _ChatterPin_h_
#define _ChatterPin_h_

#include "MofN_Filter.h"
#include "TimeBlockFilter.h"

// Zweistufiger Filter aus A) Tiefpass und B) Sperrzeit
// ####################################################
//
// Dieser Filter nimmt an, dass
// 1) die Abtastungen ann�hernd konstante Abtastraten
//    haben und
// 2) folgender Signalverlauf vorliegt:
//
//     |--|          |-|  |----|   |-------------| |-------
// ----|  |----------| |--|    |---|             |-|
//    St�rung           Prellzeit              St�rung
//
// A) m aus n Tiefpass: Liefert 1 wenn mindestens 3 aus 5
//    Abtastungen(in der Vergangenheit) eine eins liefern
//    falls das Signal vorher eine Null war und umgekehrt,
//    falls es vorher eine Eins war.
//    Damit ergibt sich im obigen Beispiel dieses Signal:
//                           |----|    |-----------------
// --------------------------|    |----|
//
// B) Interrupt bei �nderung ausl�sen und diesen f�r eine
//    Sperrzeit sperren:
//
// --------------------------V++++V++++V++++-------------
//                           V  Sperrzeit
//                           V
//                           V
//                    Interrupt ausl�sen
//
// Die Faktoren m und n sind konfigurierbar und auch die Sperrzeit.
// Damit sind Stoerungen, sofern sie deutlich kuerzere Zeiten als die
// Sperrzeit haben eindeutig filterbar und gleichzeitig kann die
// Umchaltung detektiert werden bevor die Prellzeit zu ende ist.
// --> Theoretisch kuerzeste Erkennungszeitspanne wird erreicht.
//
// Sind die Abtastzeiten unregelmaessig werden zwei Effekte tragend:
// 1) Die Zeiten veraendern sich in Richtung "Worst Case". Amax steht
// fuer maximale Zeit zwischen zwei Abtastungen, Amin fuer die minimale:
// Der Faktor m wird ueber
//   TStoerung < m * Amin
// bestimmt. Die kuerzest moegliche Abtastzeit bestimmt also m.
// Damit ergibt sich auch n und damit die Sperrzeit, weil ja
//   Sperrzeit > n * Amax
// sein muss.

#define TRIGGER_DN  0x00      
#define TRIGGER_UP  0x10      
#define TRIGGER_ANY 0x20      

class ChatterPin
{
   unsigned char myID;
   unsigned char pinAndTriggermode;

   // Tiefpassfilter
   MofN_Filter derMofN_Filter;

   // Zeitfilter
   TimeBlockFilter derTimeBlockFilter;

   // Event erkennen
   unsigned char signalVorher;

public:
   void setID(unsigned char dieID){myID=dieID;};
   unsigned char getID(){return myID;};
   
   ChatterPin( unsigned char pin,
               unsigned char trigger_UpDnAny,
               unsigned long sperrzeitMikroSec,
               unsigned char n,
               unsigned char outOfm); // Siehe Doku oben!

   unsigned char getPin();
   unsigned char checkEvent(char value = -1); // Muss regelmaessig aufgerufen werden.
                                              // Wenn Argument = -1, dann wird der Pin gepollt.
                                              // Wenn Argument = 0 oder 1, dann wird dieser Wert verwendet.
                                              // Liefert 1 wenn durch trigger_UpDnAny
                                              // eingestelltes Event aufgetreten ist,
                                              // andernfalls 0.
};

#endif
