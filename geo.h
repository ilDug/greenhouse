#ifndef GEO_H
#define GEO_H

#include "Arduino.h"
#include "dag_timer.h"


enum GEO_STATES { COLD,   // terreno freddo
                   HOT };  // terreno sufficientemente CALDO

class Geo {
private:
  Stream* srl;
  DagTimer tmr;

  int SENS_PIN;                              // ANALOG  pin per la lettura della temperatura del suolo.
  int THRESHOLD;                             // valore limite della temperatura per innescare il riscaldamento.
  int geoValue();                           // valore del sensore su scala  0-1023. Legge il valore del sensore di umidità
  const int delta = 75;                      // delta dell'umidità per controllare l'isteresi (in percentuale)
  const unsigned long frequency = 1000 * 5;  // frequenza di aggiornamento della lettura del sdensore di umidità
  bool isCold();                             // controlla se il terreno ha bisogno di acqua


public:
  Geo(int tempSensPin, Stream* _srl);  // constructor
  void run(int tempThreshold);                                    // avvia il controller del suolo. impostando la soglia di umidità e la soglia di temperatura.
  GEO_STATES STATUS;
};

#endif