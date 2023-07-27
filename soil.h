#ifndef SOIL_H
#define SOIL_H

#include "Arduino.h"
#include "dag_timer.h"

class Soil {
private:
  enum STATES { HEALTHY,                           // situazione regolare
                DRY,                               // terreno secco
                WATERING,                          // irrigazione in corso
                EMPTY_TANK };                      // serbatoio vuoto
  STATES STATUS;                                   // variabile che salva lo stato dell'istanza.
  bool isDry();                                    // controlla se il terreno ha bisogno di acqua
  bool isTankEmpty();                              //controlla se il serbatoio dell'acqua è vuoto
  int PUMP_PIN;                                    // DIGITAL pin per l'avvio della pompa di irrigazione.
  int TANK_LEVEL_PIN;                              // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio. (HIGH => empty?????)
  int SOIL_HUM_PIN;                                // ANALOG  pin per la lettura dell'umidità del suolo.
  int SOIL_HUM_THRESHOLD;                          // valore limite dell'umidità per innescare l'irrigazione. su scala 0-1023
  int soilHumValue();                              // valore del sensore su scala  0-1023. Legge il valore del sensore di umidità
  unsigned long wateringTime = (1000 * 20);  // il tempo minimo per cui la pompa deve rimanere attiva in millisecondi
  DagTimer wt;                                     // il timer non bloccante attivo per la durata dell'irrigazione
  void watering();                                 // avvia l'irrigazione
  void setHumidityThreshold(int thresold);         // imposta la soglia di umidità minima sotto la quale il terreno ha bisogno di acqua.

public:
  Soil(int humSensPin, int tankLevelPin, int pumpPin);  // constructor
  void run(int humThreshold);                           // avvia il controller del suolo. impostando la soglia di umidità.
};

#endif