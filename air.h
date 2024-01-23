#ifndef TERMO_IGRO_H
#define TERMO_IGRO_H

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "dag_timer.h"

class TermoIgro {
private:
  DHT* dht;

public:
  TermoIgro(DHT* _dht);  // constructor che prende come argomento la durata del timer
  int hum = 0;           // valore dell'umidità
  int temp = 0;          // valore di temperature in gradi Celsius
  void listen();
};

#endif