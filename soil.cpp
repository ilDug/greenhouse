#include "Arduino.h"
#include "soil.h"
#include "dag_timer.h"
#include "moisture.h"
#include "geo.h"

Soil::Soil(int tankLevelPin, int pumpPin, int heatPin, Stream* _srl)
  : tmr() {
  srl = _srl;
  TANK_LEVEL_PIN = tankLevelPin;
  PUMP_PIN = pumpPin;
  SOIL_HEAT_PIN = heatPin;
}


void Soil::run(Moisture* m, Geo* g) {
  // controller del pad riscaldante
  if (g->STATUS == COLD) {
    digitalWrite(SOIL_HEAT_PIN, HIGH);
  } else if (g->STATUS == HOT) {
    digitalWrite(SOIL_HEAT_PIN, LOW);
  }

  //controlla il serbatoio
  if (isTankEmpty()) STATUS = EMPTY_TANK;

  switch (STATUS) {

    case WATERING:
      if (waterLock) {                // se è attivo il blocco del'irrigazione
        digitalWrite(PUMP_PIN, LOW);  //spegne la pompa
      } else {
        if (tmr.clock() && m->STATUS == WET) {  // quando il timer scade
          digitalWrite(PUMP_PIN, LOW);          //spegne la pompa
          STATUS = HEALTHY;                     // imposta provvisoriamento lo stato in HEALHY  e lo ricontrolla immediatamente
        }
      }
      break;

    case HEALTHY:
      if (waterLock) {                // se è attivo il blocco del'irrigazione
        digitalWrite(PUMP_PIN, LOW);  //spegne la pompa
      } else {
        if (m->STATUS == DRY) {
          STATUS = WATERING;              // imposta lo stato in irrigazione
          tmr.init(wateringTime, false);  //inizzializza il timer eseguito una volta sola
          digitalWrite(PUMP_PIN, HIGH);   //accende la pompa
        }
      }
      break;

    case EMPTY_TANK:
      digitalWrite(PUMP_PIN, LOW);  //se il serbatoio è vuoto,  interrompe l'irrigazione
      // ACCENDE l'allarme
      break;
  }
}


bool Soil::isTankEmpty() {
  int lowLevel = digitalRead(TANK_LEVEL_PIN);
  return lowLevel == LOW;
}


void Soil::lockWatering() {
  waterLock = !waterLock;
}