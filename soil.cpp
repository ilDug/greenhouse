#include "Arduino.h"
#include "soil.h"
#include "dag_timer.h"

Soil::Soil(int humSensPin, int tankLevelPin, int pumpPin, int tempSensPin, int heatPin)
  : wt() {
  SOIL_HUM_PIN = humSensPin;
  TANK_LEVEL_PIN = tankLevelPin;
  PUMP_PIN = pumpPin;
  SOIL_TEMP_PIN = tempSensPin;
  SOIL_HEAT_PIN = heatPin;
  DagTimer wt();

  tempStatus = &TEMP_STATUS;
  humStatus = &HUM_STATUS;
}


void Soil::run(int humThreshold, int tempThreshold) {
  //controlla il serbatoio
  if (isTankEmpty()) HUM_STATUS = EMPTY_TANK;
  setHumidityThreshold(humThreshold);

  switch (HUM_STATUS) {

    case WATERING:
      watering();
      break;

    case HEALTHY:
      if (isDry()) HUM_STATUS = DRY;
      break;

    case DRY:
      if (!isDry()) HUM_STATUS = HEALTHY;
      else watering();
      break;

    case EMPTY_TANK:
      digitalWrite(PUMP_PIN, LOW);  //se il serbatoio è vuoto,  interrompe l'irrigazione
      // ACCENDE l'allarme
      break;
  }

  setTemperatureThreshol(tempThreshold);
  heating();
}


void Soil::setHumidityThreshold(int thresold) {
  SOIL_HUM_THRESHOLD = thresold;
}

void Soil::setTemperatureThreshol(int threshold) {
  SOIL_TEMP_THRESHOLD = threshold;
}


void Soil::watering() {
  if (waterLock) {                  // se è attivo il blocco del'irrigazione
    digitalWrite(PUMP_PIN, LOW);    //spegne la pompa
    if (isDry()) HUM_STATUS = DRY;  // ricontrollo lo stato
    return;
  }

  if (HUM_STATUS != WATERING) {
    // se lo stato precedente non era in fase di irriazione
    HUM_STATUS = WATERING;          // imposta lo stato in irrigazione
    wt.init(wateringTime, false);   //inizzializza il timer eseguito una volta sola
    digitalWrite(PUMP_PIN, HIGH);   //accende la pompa
  } else {                          // siamo nel caso in cui la pompa è già partita
    if (wt.clock() && !isDry()) {   // quando il timer scade
      digitalWrite(PUMP_PIN, LOW);  //spegne la pompa
      HUM_STATUS = HEALTHY;         // imposta provvisoriamento lo stato in HEALHY  e lo ricontrolla immediatamente
    }
  }
}

bool Soil::isDry() {
  switch (HUM_STATUS) {
    case HEALTHY:
    case EMPTY_TANK:
      // il valore di umidità scende sotto la soglia diminuita del delta.
      return (soilHumValue() < (SOIL_HUM_THRESHOLD - du));

    case DRY:
    case WATERING:
      //  il valore di umidità sale sopra la soglia aumentata del delta.
      return (soilHumValue() < (SOIL_HUM_THRESHOLD + du));
  }
}


bool Soil::isTankEmpty() {
  int lowLevel = digitalRead(TANK_LEVEL_PIN);
  return lowLevel == HIGH;
}

int Soil::soilHumValue() {
  return analogRead(SOIL_HUM_PIN);
}

int Soil::soilTempValue() {
  return analogRead(SOIL_TEMP_PIN);
}


bool Soil::isCold() {
  switch (TEMP_STATUS) {
    case HOT:
      return (soilTempValue() < (SOIL_TEMP_THRESHOLD - dt));
      break;
    case COLD:
      return (soilTempValue() < (SOIL_TEMP_THRESHOLD + dt));
      break;
  }
}

void Soil::heating() {
  if (isCold()) {
    TEMP_STATUS = COLD;
    digitalWrite(SOIL_HEAT_PIN, HIGH);
  }
  if (!isCold()) {
    TEMP_STATUS = HOT;
    digitalWrite(SOIL_HEAT_PIN, LOW);
  }
}

bool Soil::isWatering() {
  return HUM_STATUS == WATERING;
}

bool Soil::isHeating() {
  return TEMP_STATUS == COLD;
}

void Soil::lockWatering(bool lock) {
  waterLock = lock;
}