#include "ArduinoAppBahnanlage.h"
#include "ArduinoMain.h"

#include "Lok.h"
#include "Weiche.h"
#include "Wagenabkoppler.h"
#include "Lichtsensor.h"
#include "Laser.h"

#define laserWiederstand 800


ArduinoAppBahnanlage::ArduinoAppBahnanlage(ArduinoMain* eineArduinoMain)
: ArduinoApp(eineArduinoMain){
}

void ArduinoAppBahnanlage::appSetup(){
  meineLok               = new Lok(9,8);
  meineLok->setGeschwindigkeit(180);
  meineWeiche            = new Weiche(4,5);
  meineWagenabkoppler[0] = new Wagenabkoppler(6);
  meineWagenabkoppler[1] = new Wagenabkoppler(7);
  meineLichtsensoren[0]  = new Lichtsensor(A0);
  meineLichtsensoren[1]  = new Lichtsensor(A1);
  meineLichtsensoren[2]  = new Lichtsensor(A2);
  meineLichtsensoren[3]  = new Lichtsensor(A3);
  meineLichtsensoren[4]  = new Lichtsensor(A4);
  meineLaser             = new Laser(12);

  //Timer
  baseTimer              = meinArduinoKernel->setTimerInterrupt(20000);
  laserCheckTimer        = 0;
  warteTimer             = 0;
  weichenTimer           = 0;
  mussWarten             = false;
  weichenImpuls          = false;
  debugTimer             = 0;

}

void ArduinoAppBahnanlage::appLoop(){

}

bool ArduinoAppBahnanlage::appEvent(int idEvent){
  if ( idEvent == baseTimer){
    //Timervariablen
    laserCheckTimer++;
    debugTimer++;
    if ( mussWarten ){
      warteTimer++;
    }
    if ( weichenImpuls){
      weichenTimer++;
    }

    //Laser-Pulsen
    meineLaser->laserSwitch();

    //Lichtsensoren messen
    for ( int i = 0 ; i < 5 ; i++){
      messwerte[i] = meineLichtsensoren[i]->read();
    }


    //Laser checken
    if ( laserCheckTimer >= 5 ){
      if ( !mussWarten ){
          laserCheckTimer = 0;
      if ( messwerte[0] > laserWiederstand ){
          meineLok->switchFahrtRichtung();
          meineWeiche->switchRichtung();
          weichenImpuls = true;
          mussWarten = true;
      }
      else if ( messwerte[3] > laserWiederstand ){
        meineLok->switchFahrtRichtung();
        mussWarten = true;
      }
      else if ( messwerte[4] > laserWiederstand ){
        meineLok->switchFahrtRichtung();
        mussWarten = true;
      }
      laserCheckTimer = 0;
    }
  }

  if ( warteTimer >= 150 ){
    warteTimer = 0;
    mussWarten = false;
  }

  if ( weichenTimer >= 25 ){
    weichenTimer = 0;
    meineWeiche->timerEventPulsEnd();
    weichenImpuls = false;
  }

  if ( debugTimer >= 10 ){
    debugTimer = 0;
    for ( int i = 0 ; i < 5 ; i++){
      Serial.print("Laser");
      Serial.println(i);
      Serial.println(messwerte[i]);
    }

  }
  return true;
   }
  else{
    return false;
  }
}
