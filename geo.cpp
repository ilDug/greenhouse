#include <Arduino.h>
#include "dag_timer.h"
#include "geo.h"

Geo::Geo(DallasTemperature* _sensor, Stream* _srl)
  : tmr() {
  tmr.init(frequency);
  srl = _srl;
  sensor = _sensor;
}


void Geo::run(int tempThreshold) {
  THRESHOLD = map(tempThreshold, 0, 1024, 0, 35);  // imposta la soglia

  if (tmr.clock()) {
    STATUS = isCold() ? COLD : HOT;
  }
}

int Geo::geoValue() {
  sensor->requestTemperatures();
  temp = sensor->getTempCByIndex(0);
  srl->print("<Geo>: ");
  srl->println(temp);
  return int(temp);
}



bool Geo::isCold() {
  switch (STATUS) {
    case COLD:
      // il valore di umidità scende sotto la soglia diminuita del delta.
      return (geoValue() < (THRESHOLD + delta));

    case HOT:
      //  il valore di umidità sale sopra la soglia aumentata del delta.
      return (geoValue() < (THRESHOLD - delta));
  }
}