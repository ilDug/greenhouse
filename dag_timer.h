#ifndef DAG_TIMER_H
#define DAG_TIMER_H

#include "Arduino.h"

class DagTimer {
private:
  unsigned long bookmark;   // variabile che salva il time per il confronto del loop successivo
  unsigned long duration;   // Durata del timer in millisecondi
  void (*call_back)(void);  // funzione da eseguire allo scattare del timer
  int FIRED;                // indica se il timer è attivo (sta eseguendo la funzione di callback)
  boolean LOOP;             //indica se il timer deve ripetersi ciclicament

public:
  DagTimer();                              // constructor che prende come argomento la durata del timer
  void init(unsigned long time_duration);  // inizializza (specificare o meno se è da eseguire in loop. DEFAULT TRUE)
  void init(unsigned long time_duration, bool repeat);
  bool triggered();             // restituisce un booleano che verifica se il periodo è scoccato.
  bool clock();                 // alias per triggered
  void run(void (*fun)(void));  // avvia il timer,  ed esegue la funzione passata come argomento
};

#endif