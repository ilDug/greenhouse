#ifndef SOIL_H
#define SOIL_H

#include "Arduino.h"
#include "dag_timer.h"

enum HUM_STATES { HEALTHY,       // situazione regolare
                  DRY,           // terreno secco
                  WATERING,      // irrigazione in corso
                  EMPTY_TANK };  // serbatoio vuoto

enum TEMP_STATES { HOT,     // caldo regolare
                   COLD };  // terreno freddo


class Soil {
private:

  HUM_STATES HUM_STATUS;    // variabile che salva lo stato dell'UMIDITÀ.
  TEMP_STATES TEMP_STATUS;  //variabile che salva lo stato della TEMPERATURA

  int PUMP_PIN;             // DIGITAL pin per l'avvio della pompa di irrigazione.
  int TANK_LEVEL_PIN;       // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio. (HIGH => empty?????)
  int SOIL_HUM_PIN;         // ANALOG  pin per la lettura dell'umidità del suolo.
  int SOIL_HUM_THRESHOLD;   // valore limite dell'umidità per innescare l'irrigazione. su scala 0-1023
  int SOIL_TEMP_PIN;        // ANALOG pin del sensore di temperatura del terreno
  int SOIL_TEMP_THRESHOLD;  // limite di temperatura per innescare il riscaldamento
  int SOIL_HEAT_PIN;        // DIGITAL pin per attivare il riscaldamento


  const int dt = 1;                                // delta temperatura per controllare l'isteresi
  const int du = 100;                              // delta dell'umidità per controllare l'isteresi
  const unsigned long wateringTime = (1000 * 20);  // il tempo minimo per cui la pompa deve rimanere attiva in millisecondi
  DagTimer wt;                                     // il timer non bloccante attivo per la durata dell'irrigazione

  void setHumidityThreshold(int thresold);     // imposta la soglia di umidità minima sotto la quale il terreno ha bisogno di acqua.
  void setTemperatureThreshol(int threshold);  // imposta la soglia di tmperatura minima sotto la quale si ativa il riscaldamento.
  int soilHumValue();                          // valore del sensore su scala  0-1023. Legge il valore del sensore di umidità
  int soilTempValue();                         // legge il valore del sensore di temperatura del terreno
  void watering();                             // avvia l'irrigazione
  void heating();                              // avvia il riscaldamento in base alal soglia impostata.

  bool waterLock = false;  // se impostato il blocco , impedisce l'irrigazione

public:
  Soil(int humSensPin, int tankLevelPin, int pumpPin, int tempSensPin, int heatPin);  // constructor
  void run(int humThreshold, int tempThreshold);                                      // avvia il controller del suolo. impostando la soglia di umidità e la soglia di temperatura.

  HUM_STATES* humStatus;
  TEMP_STATES* tempStatus;

  bool isDry();        // controlla se il terreno ha bisogno di acqua
  bool isCold();       // controlla che il terreno sia in temperatura;
  bool isTankEmpty();  // controlla se il serbatoio dell'acqua è vuoto
  bool isWatering();   // indica se è attiva l'irrigazione
  bool isHeating();    // indic se è attivo il riscaldamento

  void lockWatering(bool lock);  // imposta il blocco della pompa di irrigazione in base al valore passato.
};

#endif