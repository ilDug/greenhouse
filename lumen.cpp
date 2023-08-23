#include "lumen.h"
#include "Arduino.h"


Lumen::Lumen(int lampPin, int sensorPin, Stream *_srl)
  : t() {
  LAMP_PIN = lampPin;
  SENS_PIN = sensorPin;
  srl = _srl;
  t.init(1000);
}


void Lumen::on() {
  STATUS = LAMP_ON;
  AUTO_MODE = !AUTO_MODE;
}


void Lumen::off() {
  STATUS = LAMP_OFF;
  AUTO_MODE = !AUTO_MODE;
}

void Lumen::toggle() {
  AUTO_MODE = false;
  if (STATUS == LAMP_ON) { STATUS = LAMP_OFF; }
  else if (STATUS == LAMP_OFF) { STATUS = LAMP_ON; }
}


void Lumen::run(int threshold) {
  LIGHT_THRESHOLD = threshold;

  switch (STATUS) {
    case LAMP_ON:
      digitalWrite(LAMP_PIN, HIGH);
      break;

    case LAMP_OFF:
      digitalWrite(LAMP_PIN, LOW);
      break;
  }

  if (AUTO_MODE) {
    if (t.clock()) {
      LightStates newStatus = isDark() ? LAMP_ON : LAMP_OFF;
      STATUS = newStatus;
    }
  }
}


bool Lumen::isDark() {
  switch (STATUS) {
    case LAMP_ON:
      return lumenValue() < (LIGHT_THRESHOLD + delta);
      break;

    case LAMP_OFF:
    default:
      return lumenValue() < (LIGHT_THRESHOLD - delta);
      break;
  }
}


int Lumen::lumenValue() {
  int n = 0, i = 0, m = 0, s = 0;
  while (n <= 10 && i < 16) {
    i++;
    int v = analogRead(SENS_PIN);
    if (isnan(v) == false) {
      s += v;
      n++;
    }
  }
  m = int(s / n);
  value = m;
  srl->print("<Lumen>: ");
  srl->println(m);
  return m;
}