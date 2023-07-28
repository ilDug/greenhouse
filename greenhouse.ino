/**
 * SERRA AUTOMATIZZATA 
 * DAG PROJECT
 * dag-greenhouse
 * copyright 2023
*/

#include "dag_timer.h"
#include "dag_button.h"
#include "soil.h"
#include "lumen.h"
#include "termo_igro.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/** costanti per l'illuminazione */
const int LUMEN_THRESHOLD = 0;  // valore limite minimo del sensore di luminosità per l'accensione della luce.
const int LUMEN_PIN = 0;        // pin per la lettura del sensore di luminosità.
const int LAMP_PIN = 0;         // DIGITAL pin di attivazione della luce, collegato al relay.
Lumen lumen(LAMP_PIN, LUMEN_PIN);


/** costanti per la termo-igrometria */
const int DHT_PIN = 0;  // pin per la lettura del sensore di umidità e temperatura dell'aria.
// #define DHTTYPE DHT22   // DHT 22 (AM2302)
#define DHTTYPE DHT21  // DHT 21 (AM2301)
DHT dht(DHT_PIN, DHTTYPE);
TermoIgro ht(&dht);

/** costanti per il controllo dell'irrigazione */
const int PUMP_PIN = 0;             // DIGITAL pin per l'avvio della pompa di irrigazione.
const int TANK_LEVEL_PIN = 0;       // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio.
const int SOIL_HUM_PIN = 0;         // ANALOG  pin per la lettura dell'umidità del suolo.
const int SOIL_TEMP_PIN = 0;        // ANALOG pin del sensore di temperatura del terreno
const int SOIL_HEAT_PIN = 0;        // DIGITAL pin per attivare il riscaldamento
const int SOIL_TEMP_THRESHOLD = 0;  // limite di temperatura per innescare il riscaldamento
const int SOIL_HUM_THRESHOLD = 0;   // valore limite dell'umidità per innescare l'irrigazione.
Soil soil(SOIL_HUM_PIN, TANK_LEVEL_PIN, PUMP_PIN, SOIL_TEMP_PIN, SOIL_HEAT_PIN);

void setup() {
  Serial.begin(9600);
  dht.begin();

  /** IRRIGAZIONE */
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(TANK_LEVEL_PIN, INPUT_PULLUP);
  pinMode(SOIL_HUM_PIN, INPUT);
  pinMode(SOIL_TEMP_PIN, INPUT);
  pinMode(SOIL_HEAT_PIN, OUTPUT);
  pinMode(LUMEN_PIN, INPUT);
  pinMode(LAMP_PIN, OUTPUT);
}


void loop() {

  // PROCESSI //
  /** check/manutenzione del sistema  */

  /** controllo suolo */
  soil.run(SOIL_HUM_THRESHOLD, SOIL_TEMP_THRESHOLD);

  /** controllo illuminazione */
  lumen.run(LUMEN_THRESHOLD);

  /** controllo termo-igrometria */
  ht.run();
}
