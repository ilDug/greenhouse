#ifndef SOIL_H
#define SOIL_H

#include "Arduino.h"
#include "dag_timer.h"
#include "moisture.h"
#include "geo.h"

enum WATERING_STATES { HEALTHY,       // situazione regolare
                       WATERING,      // irrigazione in corso
                       EMPTY_TANK };  // serbatoio vuoto


class Soil {
private:
  DagTimer tmr_on;   // il timer non bloccante attivo per la durata dell'irrigazione
  DagTimer tmr_off;  // il timer non bloccante attivo per la pausa tra un irriazione e l'altra.
  Stream* srl;       //serial  monitor reference

  int PUMP_PIN;        // DIGITAL pin per l'avvio della pompa di irrigazione.
  int TANK_LEVEL_PIN;  // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio. (HIGH => empty?????)
  int SOIL_HEAT_PIN;   // DIGITAL pin per attivare il riscaldamento

  const unsigned long wateringTime = (1000 * 5);  // il tempo minimo per cui la pompa deve rimanere attiva in millisecondi
  bool isTankEmpty();

public:
  Soil(int tankLevelPin, int pumpPin, int heatPin, Stream* _srl);  // constructor
  void run(Moisture* m, Geo* g);                                   // avvia il controller del suolo. impostando la soglia di umidità e la soglia di temperatura.
  WATERING_STATES STATUS;                                          //variabile che salva lo stato della TEMPERATURA

  bool waterLock = false;  // se impostato il blocco , impedisce l'irrigazione
  void lockWatering();     // imposta il blocco della pompa di irrigazione in base al valore passato.
};

#endif