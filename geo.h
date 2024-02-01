#ifndef GEO_H
#define GEO_H

#include "Arduino.h"
#include "dag_timer.h"
#include <DallasTemperature.h>
#include <OneWire.h>

enum GEO_STATES { COLD,   // terreno freddo
                  HOT };  // terreno sufficientemente CALDO

class Geo {
private:
  Stream* srl;                // puntatore al Serial monitor
  DagTimer tmr;               // Istanza dell'oggetto DagTimer
  DallasTemperature* sensor;  //puntatore all'istanza dell oggetto DS18B20

  int THRESHOLD;                             // valore limite della temperatura per innescare il riscaldamento.
  int geoValue();                            // valore del sensore in gradi Celsius
  const int delta = 3;                      // delta dell'umidità per controllare l'isteresi (in bit)
  const unsigned long frequency = 1000 * 5;  // frequenza di aggiornamento della lettura del sdensore di temperatura
  bool isCold();                             // controlla se il terreno ha bisogno di acqua

public:
  Geo(DallasTemperature* _sensor, Stream* _srl);  // constructor
  void run(int tempThreshold);                    // avvia il controller del suolo. impostando la soglia di umidità e la soglia di temperatura.
  float temp;                                     // variabile che controlla le letture della temperatura
  GEO_STATES STATUS;
};

#endif