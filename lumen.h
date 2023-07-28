#ifndef LUMEN_H
#define LUMEN_H

#include "Arduino.h"

enum LightStates { ON,
                   OFF,
                   AUTO };

class Lumen {
private:
  LightStates STATUS;     // stato del sistema di regolazione
  int LAMP_PIN;           // DIGITAL pin di attivazione della luce
  int SENS_PIN;           // ANALOG pin del sensore di luminosità
  const int delta = 100;  // delta della soglia per controllare l'isteresi del sensore
  bool isDark();          // controlla la luminosità esterna
  int LIGHT_THRESHOLD;    // soglia di luminosità
  int lumenValue();       // lettura del valore di luminosità

public:
  Lumen(int lampPin, int sensorPin);  // constructor
  void run(int threshold);            // esegue il controllo della luce con la soglia minima impostabile
  void on();                          // accende in maniera forzata
  void off();                         // spegne in maniera forzata
};

#endif