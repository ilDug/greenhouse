#ifndef TERMO_IGRO_H
#define TERMO_IGRO_H

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "dag_timer.h"

class TermoIgro {
private:
  DagTimer timer;
  DHT* dht;
  const int frequency = 2000;

public:
  TermoIgro(DHT* _dht);  // constructor che prende come argomento la durata del timer
  float h;
  float t;
  void run();
};

#endif