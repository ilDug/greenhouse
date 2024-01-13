#include "air.h"

TermoIgro::TermoIgro(DHT* _dht) {
  dht = _dht;
}


void TermoIgro::listen() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    int hn = 0, tn = 0, i = 0;  // numero leture  di umidità,  temperatura e totali
    float hs = 0, ts = 0;       // somma delle leture
    float _h, _t;               // lettura istantanea

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
    hum = int(hs / hn);
    temp = int(ts / tn);
}
