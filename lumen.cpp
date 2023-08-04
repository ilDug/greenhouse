#include "lumen.h"
#include "Arduino.h"


Lumen::Lumen(int lampPin, int sensorPin)
  : t() {
  LAMP_PIN = lampPin;
  SENS_PIN = sensorPin;
  // DagTimer t();
  t.init(1000);
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
      if (t.clock()) {
        int command = isDark() ? HIGH : LOW;
        digitalWrite(LAMP_PIN, command);
      }
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
  int n, i;
  float s, m;
  while (n <= 10 && i < 16) {
    i++;
    int v = analogRead(SENS_PIN);
    if (!isnan(v)) {
      s += v;
      n++;
    }
  }
  m = s / n;
  return m;
}