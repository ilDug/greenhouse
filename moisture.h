#ifndef MOISTURE_H
#define MOISTURE_H

#include "Arduino.h"
#include "dag_timer.h"


enum MOISTURE_STATES { DRY,    // terreno secco
                       WET };  // terreno sufficientemente umido

class Moisture {
private:
  Stream* srl;
  DagTimer tmr;

  int SENS_PIN;                               // ANALOG  pin per la lettura dell'umidità del suolo.
  int ENABLE_PIN;                             // DIGITAL pin per l'attivazione del transistore che abilita il sensore
  int THRESHOLD;                              // valore limite dell'umidità per innescare l'irrigazione.
  int moistureValue();                        // valore del sensore su scala  0-1023. Legge il valore del sensore di umidità
  const int delta = 75;                        // delta dell'umidità per controllare l'isteresi (in percentuale)
  const unsigned long frequency = 1000 * 5;  // frequenza di aggiornamento della lettura del sensore di umidità
  bool isDry();                               // controlla se il terreno ha bisogno di acqua


public:
  Moisture(int humSensPin, int humSensEnablePin, Stream* _srl);  // constructor
  void run(int humThreshold);                                    // avvia il controller del suolo. impostando la soglia di umidità e la soglia di temperatura.
  MOISTURE_STATES STATUS;
};

#endif