#include "lumen.h"
#include "Arduino.h"

Lumen::Lumen(int lampPin, int sensorPin) {
  LAMP_PIN = lampPin;
  SENS_PIN = sensorPin;
}


void Lumen::on() {
  STATUS = STATUS == ON ? AUTO : ON;
}


void Lumen::off() {
  STATUS = STATUS == OFF ? AUTO : OFF;
}

void Lumen::run(int threshold) {
  LIGHT_THRESHOLD = threshold;

  switch (STATUS) {
    case ON:
      digitalWrite(LAMP_PIN, HIGH);
      break;

    case OFF:
      digitalWrite(LAMP_PIN, LOW);
      break;

    case AUTO:
      int command = isDark() ? HIGH : LOW;
      digitalWrite(LAMP_PIN, command);
      break;
  }
}

bool Lumen::isDark() {
  switch (STATUS) {
    case ON:
      return lumenValue() < (LIGHT_THRESHOLD - delta);
      break;

    case OFF:
    default:
      return lumenValue() < (LIGHT_THRESHOLD + delta);
      break;
  }
}

int Lumen::lumenValue() {
  return analogRead(SENS_PIN);
}