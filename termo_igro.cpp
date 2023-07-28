#include "termo_igro.h"

TermoIgro::TermoIgro(DHT* _dht)
  : timer() {
  timer.init(frequency);
  dht = _dht;
}


void TermoIgro::run() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if (timer.clock()) {
    float _h = dht->readHumidity();
    float _t = dht->readTemperature();
    if (!isnan(_h)) h = _h;
    if (!isnan(_t)) t = _t;
  }
}
