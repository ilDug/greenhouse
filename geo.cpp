#include <Arduino.h>
#include "dag_timer.h"
#include "geo.h"


Geo::Geo(int tempSensPin, Stream* _srl)
  : tmr() {
  SENS_PIN = tempSensPin;  // ANALOG  pin per la lettura della temperatura del suolo.
  tmr.init(frequency);
  srl = _srl;
}


void Geo::run(int tempThreshold) {
  THRESHOLD = tempThreshold;  // imposta la soglia

  if (tmr.clock()) {
    srl->println("<Geo> measure.");
    STATUS = isCold() ? COLD : HOT;
  }
}

int Geo::geoValue() {
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
  m = map(m, 0, 1024, 1000, 0);

  srl->print("<Geo>: ");
  srl->println(m);
  return m;
}



bool Geo::isCold() {
  switch (STATUS) {
    case COLD:
      // il valore di umidità scende sotto la soglia diminuita del delta.
      return (geoValue() > (THRESHOLD + delta));

    case HOT:
      //  il valore di umidità sale sopra la soglia aumentata del delta.
      return (geoValue() < (THRESHOLD - delta));
  }
}