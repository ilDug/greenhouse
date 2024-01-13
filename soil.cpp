#include "Arduino.h"
#include "soil.h"
#include "dag_timer.h"
#include "moisture.h"
#include "geo.h"

Soil::Soil(int tankLevelPin, int pumpPin, int heatPin, Stream* _srl)
  : tmr_on(), tmr_off() {
  srl = _srl;
  TANK_LEVEL_PIN = tankLevelPin;
  PUMP_PIN = pumpPin;
  SOIL_HEAT_PIN = heatPin;
  STATUS = HEALTHY;  // inizializza come in buono stato per prevenire che la pompa si accenda subito.
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
        if (m->STATUS == WET && tmr_on.clock()) {  // quando il timer scade
          tmr_off.init(wateringTime, false);       // attiva la pausa in modo che non venga attivata la pompa per permettere la diffusione
          digitalWrite(PUMP_PIN, LOW);             //spegne la pompa
          STATUS = HEALTHY;                        // imposta provvisoriamente lo stato in HEALHY  e lo ricontrolla immediatamente
        }
      }
      break;

    case HEALTHY:
      if (waterLock) {                // se è attivo il blocco del'irrigazione
        digitalWrite(PUMP_PIN, LOW);  //spegne la pompa
      } else {
        if (m->STATUS == DRY && tmr_off.clock()) {  // accende la pompa solo se è passato il periodo di pausa
          tmr_on.init(wateringTime, false);         //inizzializza il timer eseguito una volta sola
          digitalWrite(PUMP_PIN, HIGH);             //accende la pompa
          STATUS = WATERING;                        // imposta lo stato in irrigazione
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