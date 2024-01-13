#include <Arduino.h>
#include "dag_timer.h"
#include "moisture.h"


Moisture::Moisture(int humSensPin, int humSensEnablePin, Stream* _srl)
  : tmr() {
  SENS_PIN = humSensPin;          // ANALOG  pin per la lettura dell'umidità del suolo.
  ENABLE_PIN = humSensEnablePin;  // DIGITAL pin per l'attivazione del transistore che abilita il sensore
  tmr.init(frequency);
  srl = _srl;
}


void Moisture::run(int humThreshold) {
  THRESHOLD = humThreshold;  // imposta la soglia

  if (tmr.clock()) {
    srl->println("<Moisture> measure.");
    STATUS = isDry() ? DRY : WET;
  }
}

int Moisture::moistureValue() {
  digitalWrite(ENABLE_PIN, HIGH);
  delay(1000);  // necessario er inizializzare il sensore

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
  m = map(m, 0, 1024, 1024, 0);
  value = m;
  srl->print("<Moisture>: ");
  srl->println(m);
  digitalWrite(ENABLE_PIN, LOW);
  return m;
}



bool Moisture::isDry() {
  switch (STATUS) {
    case WET:
      // il valore di umidità scende sotto la soglia diminuita del delta.
      return (moistureValue() < (THRESHOLD - delta));

    case DRY:
      //  il valore di umidità sale sopra la soglia aumentata del delta.
      return (moistureValue() < (THRESHOLD + delta));
  }
}