/**
 * SERRA AUTOMATIZZATA 
 * DAG PROJECT
 * dag-greenhouse
 * copyright 2023
*/

#include "dag_timer.h"
#include "dag_button.h"
// #include "soil.h"
#include "moisture.h"
#include "lumen.h"
#include "air.h"
#include "geo.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/** costanti per l'illuminazione */
const int LUMEN_THRESHOLD = 400;  // valore limite minimo del sensore di luminosità per l'accensione della luce.
const int LUMEN_PIN = A0;         // pin per la lettura del sensore di luminosità.
const int LAMP_PIN = 8;           // DIGITAL pin di attivazione della luce, collegato al relay.
Lumen lumen(LAMP_PIN, LUMEN_PIN, &Serial);
DagButton btnLamp(8, LOW);

/** costanti per la termo-igrometria */
// const int DHT_PIN = 0;  // pin per la lettura del sensore di umidità e temperatura dell'aria.
// #define DHTTYPE DHT22   // DHT 22 (AM2302)
// #define DHTTYPE DHT21  // DHT 21 (AM2301)
// DHT dht(DHT_PIN, DHTTYPE);
// TermoIgro ht(&dht);

/** costanti per il controllo dell'irrigazione */
const int SOIL_HUM_PIN = A1;         // ANALOG  pin per la lettura dell'umidità del suolo.
const int SOIL_HUM_ENABLE_PIN = 9;   // DIGITAL pin per l'attivazione del transistore che abilita il sensore
const int SOIL_HUM_THRESHOLD = 400;  // valore limite dell'umidità per innescare l'irrigazione.
Moisture moisture(SOIL_HUM_PIN, SOIL_HUM_ENABLE_PIN, &Serial);

// const int PUMP_PIN = 0;             // DIGITAL pin per l'avvio della pompa di irrigazione.
// const int TANK_LEVEL_PIN = 0;       // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio.

const int SOIL_TEMP_PIN = A2;        // ANALOG pin del sensore di temperatura del terreno
const int SOIL_HEAT_PIN = 7;         // DIGITAL pin per attivare il riscaldamento, collegato al relay
const int SOIL_TEMP_THRESHOLD = 20;  // limite di temperatura per innescare il riscaldamento
Geo eartTemp(SOIL_HEAT_PIN, &Serial);

void setup() {
  Serial.begin(9600);
  Serial.println("Inizializzazione DAG GREENHOUSE");
  // dht.begin();

  /** IRRIGAZIONE */
  // pinMode(PUMP_PIN, OUTPUT);
  // pinMode(TANK_LEVEL_PIN, INPUT_PULLUP);
  pinMode(SOIL_HUM_PIN, INPUT);
  pinMode(SOIL_HUM_ENABLE_PIN, OUTPUT);
  // pinMode(SOIL_HEAT_PIN, OUTPUT);

  /** umidità terreno */
  pinMode(LUMEN_PIN, INPUT);
  pinMode(LAMP_PIN, OUTPUT);

  /** TEMPERATURA DEL TERRENO*/
  pinMode(SOIL_TEMP_PIN, INPUT);

}


void loop() {
  delay(10);
  // PROCESSI //
  /** check/manutenzione del sistema  */

  /** controllo suolo */
  // soil.run(SOIL_HUM_THRESHOLD, SOIL_TEMP_THRESHOLD);
  moisture.run(SOIL_HUM_THRESHOLD);
  eartTemp.run(SOIL_TEMP_THRESHOLD);


  /** controllo illuminazione */
  //lumen.run(LUMEN_THRESHOLD);
  btnLamp.onPress(lampToggle);
  btnLamp.onLongPress(lampAuto, 3000);

  /** controllo termo-igrometria */
  // ht.run();
}


/** accende e spegne la lampada */
void lampToggle() {
  lumen.toggle();
}


/** attiva l'auto mode della lampada*/
void lampAuto() {
  lumen.AUTO_MODE = true;
}
