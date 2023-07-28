#ifndef LUMEN_H
#define LUMEN_H

#include "Arduino.h"
#include "dag_timer.h"

enum LightStates { ON,
                   OFF,
                   AUTO };

class Lumen {
private:
  LightStates STATUS;     // stato del sistema di regolazione
  int LAMP_PIN;           // DIGITAL pin di attivazione della luce
  int SENS_PIN;           // ANALOG pin del sensore di luminosità
  const int delta = 100;  // delta della soglia per controllare l'isteresi del sensore
  bool isDark(void);      // controlla la luminosità esterna
  int LIGHT_THRESHOLD;    // soglia di luminosità
  int lumenValue(void);   // lettura del valore di luminosità
  DagTimer t;
  void lumenCtrl(void);

public:
  Lumen(int lampPin, int sensorPin);  // constructor
  void run(int threshold);            // esegue il controllo della luce con la soglia minima impostabile
  void on(void);                      // accende in maniera forzata
  void off(void);                     // spegne in maniera forzata
};

#endif