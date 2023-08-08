#include "air.h"

TermoIgro::TermoIgro(DHT* _dht)
  : timer() {
  timer.init(frequency);
  dht = _dht;
}


void TermoIgro::run() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if (timer.clock()) {
    int hn, tn, i;
    float hs, ts;
    float _h, _t;

    while (hn <= 3 && tn <= 3 && i < 5) {
      i++;
      _h = dht->readHumidity();
      _t = dht->readTemperature();
      if (!isnan(_h)) {
        hs += _h;
        hn++;
      }
      if (!isnan(_t)) {
        ts += _t;
        tn++;
      }
    }
    h = hs / hn;
    t = ts / tn;
  }
}
