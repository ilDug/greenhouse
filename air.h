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
  float hum = 0;
  float temp = 0;
  void listen();
};

#endif