/**
 * SERRA AUTOMATIZZATA 
 * DAG PROJECT
 * dag-greenhouse
 * copyright 2023
*/

#include "dag_timer.h"
#include "dag_button.h"
#include "soil.h"
#include "moisture.h"
#include "lumen.h"
#include "air.h"
#include "geo.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>
#include "LCD03.h"

#include <OneWire.h>
#include <DallasTemperature.h>


/** costanti per l'illuminazione */
int LUMEN_THRESHOLD;         // valore limite minimo del sensore di luminosità per l'accensione della luce.
const int LUMEN_PIN = A0;    // pin per la lettura del sensore di luminosità.
const int LAMP_PIN = 8;      // DIGITAL pin di attivazione della luce, collegato al relay.
const int LAMP_BTN_PIN = 5;  // DIGITAL pin di attivazione della luce, collegato al relay.
Lumen lumen(LAMP_PIN, LUMEN_PIN, &Serial);
DagButton btnLamp(LAMP_BTN_PIN, LOW);

/** costanti per la termo-igrometria */
const int DHT_PIN = 6;  // pin per la lettura del sensore di umidità e temperatura dell'aria.
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);
TermoIgro air(&dht);
DagTimer airTimer;

/** controllo dell'umidità del terreno */
const int SOIL_HUM_PIN = A6;         // ANALOG  pin per la lettura dell'umidità del suolo.
const int SOIL_HUM_ENABLE_PIN = A7;  // DIGITAL pin per l'attivazione del transistore che abilita il sensore
int SOIL_HUM_THRESHOLD;              // valore limite dell'umidità per innescare l'irrigazione.
Moisture moisture(SOIL_HUM_PIN, SOIL_HUM_ENABLE_PIN, &Serial);

/** Controllo la temperatura del terreno */
const int SOIL_TEMP_PIN = 7;  // DIGITAL pin del sensore di temperatura del terreno DS18B20
int SOIL_TEMP_THRESHOLD;      // limite di temperatura per innescare il riscaldamento
OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature ds18b20(&oneWire);  // inizializzazine sensore temperatura terreno
Geo geo(&ds18b20, &Serial);

/** Controllo Irrigazione */
const int PUMP_PIN = 9;        // DIGITAL pin per l'avvio della pompa di irrigazione.
const int WATERLOCK_PIN = 4;   // DIGITAL segnale per il blocco del'irrigazione
const int TANK_LEVEL_PIN = 2;  // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio.
const int SOIL_HEAT_PIN = 10;  // DIGITAL pin per attivare il riscaldamento, collegato al relay
Soil soil(TANK_LEVEL_PIN, PUMP_PIN, SOIL_HEAT_PIN, &Serial);

/** Potenziometri */
int SET_THS_TEMP = A1;  // pin del potenziometro per l'impostazione della soglia di temperatura suolo
int SET_THS_HUM = A2;   // pin del potenziometro per l'impostazione della soglia di umidità terreno
int SET_THS_LUX = A3;   // pin del potenziometro per l'impostazione della soglia di luminosità

/** Display LCD*/
LCD03 lcd;  // Create new LCD03 instance
const int DSPL_BTN_PIN = 3;
void display_main();
void display_thresholds();
void display_lux();
void display_heat();
void display_moisture();
void (*pages[])() = { display_main, display_thresholds, display_lux, display_heat, display_moisture };
int activePage = 0;
uint8_t lcd_backlight = LOW;  //variabile che salva l'accensione della luce del display (attivabile con longPress del displayBtn)
DagButton displayBtn(DSPL_BTN_PIN, LOW);
DagTimer displayTimer;

/** LED */
const int LED_PWR = 13;   // Power Led
const int LED_PUMP = 11;  // Led per avvsare l'avviamento della pompa di irrigazione. Lampeggia se il serbatoio dell'acqua è vuoto.
const int LED_HEAT = 12;  // led per avvisare l'accensione del pad di riscaldamento
DagTimer ledTimer;

//*********************************************************************************
void setup() {
  Serial.begin(9600);
  Serial.println("Inizializzazione DAG GREENHOUSE");

  dht.begin();          // attiva il sensore dei parametri ambientali dell'aria.
  airTimer.init(5000);  // attiva il timer per la lettura dei parametri dell'aria.

  /** IRRIGAZIONE */
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(WATERLOCK_PIN, INPUT_PULLUP);
  pinMode(TANK_LEVEL_PIN, INPUT_PULLUP);
  pinMode(SOIL_HEAT_PIN, OUTPUT);

  /** UMIDITÀ DEL TERRENO */
  pinMode(SOIL_HUM_PIN, INPUT);
  pinMode(SOIL_HUM_ENABLE_PIN, OUTPUT);

  /** ILLUMIAZIONE */
  pinMode(LUMEN_PIN, INPUT);
  pinMode(LAMP_PIN, OUTPUT);

  /** TEMPERATURA DEL TERRENO*/
  pinMode(SOIL_TEMP_PIN, INPUT);

  /** POTENZIOMETRI */
  pinMode(SET_THS_TEMP, INPUT);
  pinMode(SET_THS_HUM, INPUT);
  pinMode(SET_THS_LUX, INPUT);

  /** LED */
  pinMode(LED_PWR, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_HEAT, OUTPUT);


  // accende il power led
  digitalWrite(LED_PWR, HIGH);
  ledTimer.init(1000);

  //LCD
  lcd.begin(16, 2);  // inizializza LCD
  lcd.backlight();   // accende lo sfondo
  lcd.home();
  lcd.print("DAG Greenhouse v0.0.1");  // messaggio di benvenuto
  delay(3000);
  lcd.clear();
  lcd.noBacklight();
}


void loop() {
  // PROCESSI //
  /** lettura dei potenziometri */
  SOIL_TEMP_THRESHOLD = analogRead(SET_THS_TEMP);
  SOIL_HUM_THRESHOLD = analogRead(SET_THS_HUM);
  LUMEN_THRESHOLD = analogRead(SET_THS_LUX);


  /** controllo suolo */
  moisture.run(SOIL_HUM_THRESHOLD);
  geo.run(SOIL_TEMP_THRESHOLD);
  soil.run(&moisture, &geo);


  /** controllo illuminazione */
  lumen.run(LUMEN_THRESHOLD);
  btnLamp.onPress(lampToggle);
  btnLamp.onLongPress(lampAuto, 3000);


  /** controllo termo-igrometria dell'aria*/
  airTimer.run(termo_igro);


  /** display */
  pages[activePage]();                 // visualizza la pagina attiva
  displayBtn.onPress(display_change);  // quando premuto aumenta il numero della pagina e fa partre il timer di reset
  displayBtn.onLongPress(display_backlight, 3000);
  displayTimer.run(display_reset);  // resetta la pagina prncipale al termine dell'intervallo di tempo

  /** led */
  ledTimer.run(led_ctrl);
}

//****************************************************************************
/** accende e spegne la lampada */
void lampToggle() {
  lumen.toggle();
}


/** attiva l'auto mode della lampada*/
void lampAuto() {
  lumen.AUTO_MODE = true;
}

/** esegue le letture dei parametri dell'aria*/
void termo_igro() {
  air.listen();
}


void led_ctrl() {
  /** comanda il led dela segnale pompa */
  switch (soil.STATUS) {
    case WATERING:
      digitalWrite(LED_PUMP, HIGH);
      break;

    case EMPTY_TANK:
      digitalWrite(LED_PUMP, !digitalRead(LED_PUMP));
      break;

    default:
      digitalWrite(LED_PUMP, LOW);
      break;
  }

  /** comanda il led del pad di riscaldamento*/
  digitalWrite(LED_HEAT, digitalRead(SOIL_HEAT_PIN));
}


//******************************************************************************
void display_change() {
  activePage = activePage >= 4 ? 0 : activePage + 1;
  displayTimer.init(5000, false);
}

void display_reset() {
  activePage = 0;
}

void display_backlight() {
  lcd_backlight = !lcd_backlight;
  switch (lcd_backlight) {
    case HIGH:
      lcd.backlight();
    case LOW:
      lcd.noBacklight();
  }
}

void display_main() {
  lcd.clear();

  // prima riga
  lcd.home();
  lcd.print("Air ");
  lcd.print(air.hum);
  lcd.print("% ");
  lcd.setCursor(0, 12);
  lcd.print(air.temp);
  lcd.print("°C");

  lcd.newLine();  // va a capo

  //seconda riga
  String g = geo.STATUS == COLD ? "COLD" : (geo.STATUS == HOT ? "HOT" : "---");
  String m = moisture.STATUS == DRY ? "DRY" : (moisture.STATUS == WET ? "WET" : "---");
  lcd.print(g);
  lcd.print(" ");
  lcd.print(m);
  lcd.setCursor(1, 12);
  lcd.print(geo.temp);
  lcd.print("°C");
}

void display_thresholds() {
  lcd.clear();
  lcd.home();

  lcd.print("Lux  Moist  Heat");
  lcd.newLine();

  lcd.print(LUMEN_THRESHOLD);
  lcd.setCursor(1, 6);
  lcd.print(SOIL_HUM_THRESHOLD);
  lcd.setCursor(1, 12);
  lcd.print(SOIL_TEMP_THRESHOLD);
}

void display_moisture() {
  lcd.clear();
  lcd.home();

  lcd.print("MOISTURE ");
  lcd.print(moisture.value);

  lcd.newLine();

  lcd.print("Threshod ");
  lcd.print(SOIL_HUM_THRESHOLD);
}

void display_lux() {
  lcd.clear();
  lcd.home();

  lcd.print("LUMEN ");
  lcd.print(lumen.value);

  lcd.newLine();

  lcd.print("Threshod ");
  lcd.print(LUMEN_THRESHOLD);
}


void display_heat() {
  lcd.clear();
  lcd.home();

  lcd.print("HEAT ");
  lcd.print(map(geo.temp, 0, 35, 0, 1024));

  lcd.newLine();

  lcd.print("Threshod ");
  lcd.print(SOIL_TEMP_THRESHOLD);
}
