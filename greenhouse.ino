/**
 * SERRA AUTOMATIZZATA 
 * DAG PROJECT
 * dag-greenhouse
 * copyright 2023
*/

#include "dag_timer.h"
#include "dag_button.h"
#include "soil.h"

/** costanti per l'illuminazione */
const int LUMEN_THRESHOLD = 0;  // valore limite minimo del sensore di luminosità per l'accensione della luce.
const int LUMEN_DELTA = 0;      // soglia del valore di luminosità per evitare l'isteresi della lampada.
const int LUMEN_PIN = 0;        // pin per la lettura del sensore di luminosità.
const int LIGHT_PIN = 0;        // pin per l'accensione della luce,  collegato a relay.

/** costanti per la termo-igrometria */
const int HT_PIN = 0;  // pin per la lettura del sensore di umidità e temperatura dell'aria.

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

  /** IRRIGAZIONE */
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(TANK_LEVEL_PIN, INPUT_PULLUP);
  pinMode(SOIL_HUM_PIN, INPUT);
  pinMode(SOIL_TEMP_PIN, INPUT);
  pinMode(SOIL_HEAT_PIN, OUTPUT);
}



void loop() {

  // PROCESSI //
  /** check/manutenzione del sistema  */

  /** controllo suolo */
  soil.run(SOIL_HUM_THRESHOLD, SOIL_TEMP_THRESHOLD);

  /** controllo illuminazione */

  /** controllo termo-igrometria */
}
