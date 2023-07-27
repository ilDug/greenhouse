#include "Arduino.h"
#include "soil.h"
#include "dag_timer.h"

Soil::Soil(int humSensPin, int tankLevelPin, int pumpPin)
  : wt(wateringTime) {
  SOIL_HUM_PIN = humSensPin;
  TANK_LEVEL_PIN = tankLevelPin;
  PUMP_PIN = pumpPin;
  // wateringTime = (1000 * 20);
  DagTimer wt(wateringTime);
}


void Soil::setHumidityThreshold(int thresold) {
  SOIL_HUM_THRESHOLD = thresold;
}


void Soil::run(int humThreshold) {
  //controlla il serbatoio
  if (isTankEmpty()) STATUS = EMPTY_TANK;
  setHumidityThreshold(humThreshold);

  switch (STATUS) {

    case WATERING:
      watering();
      break;

    case HEALTHY:
      if (isDry()) STATUS = DRY;
      break;

    case DRY:
      if (!isDry()) STATUS = HEALTHY;
      else watering();
      break;

    case EMPTY_TANK:
      digitalWrite(PUMP_PIN, LOW);  //se il serbatoio è vuoto,  interrompe l'irrigazione
      // ACCENDE l'allarme
      break;
  }
}

void Soil::watering() {
  if (STATUS != WATERING) {
    // se lo stato precedente non era in fase di irriazione
    STATUS = WATERING;              // imposta lo stato in irrigazione
    wt.init(false);                 //inizzializza il timer eseguito una volta sola
    digitalWrite(PUMP_PIN, HIGH);   //accende la pompa
  } else {                          // siamo nel caso in cui la pompa è già partita
    if (wt.clock() && !isDry()) {   // quando il timer scade
      digitalWrite(PUMP_PIN, LOW);  //spegne la pompa
      STATUS = HEALTHY;             // imposta provvisoriamento lo stato in HEALHY  e lo ricontrolla immediatamente
    }
  }
}

bool Soil::isDry() {
  int d = 100;  // delta applicato alal soglia che tiene conto dell'isteresi
  //cambia lo stato quando viene superata la soglia
  switch (STATUS) {
    case HEALTHY:
    case EMPTY_TANK:
      // il valore di umidità scende sotto la soglia diminuita del delta.
      return (soilHumValue() < (SOIL_HUM_THRESHOLD - d));

    case DRY:
    case WATERING:
      //  il valore di umidità sale sopra la soglia aumentata del delta.
      return (soilHumValue() < (SOIL_HUM_THRESHOLD + d));
  }
}


bool Soil::isTankEmpty() {
  int lowLevel = digitalRead(TANK_LEVEL_PIN);
  return lowLevel == HIGH;
}

int Soil::soilHumValue() {
  return analogRead(SOIL_HUM_PIN);
}