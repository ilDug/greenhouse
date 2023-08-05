#ifndef LUMEN_H
#define LUMEN_H

#include "Arduino.h"
#include "dag_timer.h"

enum LightStates { LAMP_OFF = 0,
                   LAMP_ON = 1 };

class Lumen {
private:
  int LAMP_PIN;          // DIGITAL pin di attivazione della luce
  int SENS_PIN;          // ANALOG pin del sensore di luminosità
  const int delta = 75;  // delta della soglia per controllare l'isteresi del sensore
  bool isDark(void);     // controlla la luminosità esterna
  int LIGHT_THRESHOLD;   // soglia di luminosità
  int lumenValue(void);  // lettura del valore di luminosità
  DagTimer t;
  Stream *srl;

public:
  Lumen(int lampPin, int sensorPin, Stream *_srl);  // constructor
  void run(int threshold);                          // esegue il controllo della luce con la soglia minima impostabile
  void on(void);                                    // accende in maniera forzata
  void off(void);                                   // spegne in maniera forzata
  void toggle(void);                                // alterna on e off
  LightStates STATUS;                               // stato del sistema di regolazione
  bool AUTO_MODE = true;                            // indica se la classe deve operare automaticamente o se è forzato uno stato;
};

#endif