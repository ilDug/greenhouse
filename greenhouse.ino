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
#include <LCD_I2C.h>

#include <OneWire.h>
#include <DallasTemperature.h>


/** costanti per l'illuminazione */
int LUMEN_THRESHOLD;         // valore limite minimo del sensore di luminosità per l'accensione della luce.
const int LUMEN_PIN = A0;    // pin per la lettura del sensore di luminosità.
const int LAMP_PIN = 10;     // DIGITAL pin di attivazione della luce, collegato al relay.
const int LAMP_BTN_PIN = 3;  // DIGITAL pin di attivazione della luce, collegato al relay.
Lumen lumen(LAMP_PIN, LUMEN_PIN, &Serial);
DagButton btnLamp(LAMP_BTN_PIN, LOW);

/** costanti per la termo-igrometria */
const int DHT_PIN = 13;  // pin per la lettura del sensore di umidità e temperatura dell'aria.
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
const int SOIL_TEMP_PIN = 12;  // DIGITAL pin del sensore di temperatura del terreno DS18B20
int SOIL_TEMP_THRESHOLD;       // limite di temperatura per innescare il riscaldamento
OneWire oneWire(SOIL_TEMP_PIN);
DallasTemperature ds18b20(&oneWire);  // inizializzazine sensore temperatura terreno
Geo geo(&ds18b20, &Serial);

/** Controllo Irrigazione */
const int PUMP_PIN = 9;         // DIGITAL pin per l'avvio della pompa di irrigazione.
const int WATERLOCK_PIN = 4;    // DIGITAL segnale per il blocco del'irrigazione
const int TANK_LEVEL_PIN = 11;  // DIGITAL pin per la lettura del segnale di livello minimo dell'acqua del serbatoio.
const int SOIL_HEAT_PIN = 2;    // DIGITAL pin per attivare il riscaldamento, collegato al relay
Soil soil(TANK_LEVEL_PIN, PUMP_PIN, SOIL_HEAT_PIN, &Serial);
DagButton waterLockBtn(WATERLOCK_PIN, LOW);


/** Potenziometri */
int SET_THS_TEMP = A2;  // pin del potenziometro per l'impostazione della soglia di temperatura suolo
int SET_THS_HUM = A1;   // pin del potenziometro per l'impostazione della soglia di umidità terreno
int SET_THS_LUX = A3;   // pin del potenziometro per l'impostazione della soglia di luminosità

/** Display LCD*/
LCD_I2C lcd(0x27, 16, 2);  //SDA => A4: SCL => A5
const int DSPL_BTN_PIN = 5;
void display_main();
void display_thresholds();
void display_lux();
void display_heat();
void display_moisture();
void (*pages[])() = { display_main, display_thresholds, display_lux, display_heat, display_moisture };
int activePage = 0;
uint8_t lcd_backlight = HIGH;  //variabile che salva l'accensione della luce del display (attivabile con longPress del displayBtn)
DagButton displayBtn(DSPL_BTN_PIN, LOW);
DagTimer displayTimer;

/** LED */
const int LED_PWR = 7;   // Power Led
const int LED_PUMP = 6;  // Led per avvsare l'avviamento della pompa di irrigazione. Lampeggia se il serbatoio dell'acqua è vuoto.
const int LED_HEAT = 8;  // led per avvisare l'accensione del pad di riscaldamento


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

  //LCD
  lcd.begin();      // inizializza LCD
  lcd.backlight();  // accende lo sfondo
  lcd.home();
  lcd.print("DAG Greenhouse");  // messaggio di benvenuto
  lcd.setCursor(0, 1);
  lcd.print("v0.0.4");
  delay(3000);
  lcd.clear();
  // lcd.noBacklight();
}


void loop() {
  delay(75);

  // PROCESSI //
  /** lettura dei potenziometri */
  SOIL_TEMP_THRESHOLD = analogRead(SET_THS_TEMP);
  SOIL_HUM_THRESHOLD = analogRead(SET_THS_HUM);
  LUMEN_THRESHOLD = analogRead(SET_THS_LUX);

  /** controllo suolo */
  moisture.run(SOIL_HUM_THRESHOLD);
  geo.run(SOIL_TEMP_THRESHOLD);
  soil.run(&moisture, &geo);
  waterLockBtn.onPress(lockWater);

  /** controllo illuminazione */
  lumen.run(LUMEN_THRESHOLD);
  btnLamp.onPress(lampToggle);
  btnLamp.onLongPress(lampAuto, 3000);


  /** controllo termo-igrometria dell'aria*/
  airTimer.run(termo_igro);


  /** display */
  displayBtn.onPress(display_change);  // quando premuto aumenta il numero della pagina e fa partre il timer di reset
  displayBtn.onLongPress(display_backlight, 5000);
  displayTimer.run(display_reset);  // resetta la pagina principale al termine dell'intervallo di tempo
  pages[activePage]();              // visualizza la pagina attiva

  /** led */
  ledController();
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

void lockWater(){
  soil.lockWatering();
}


void ledController() {
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
  displayTimer.init(20000, false);
}

void display_reset() {
  activePage = 0;
}

void display_backlight() {
  lcd_backlight = !lcd_backlight;
  switch (lcd_backlight) {
    case HIGH:
      lcd.backlight();
      break;
    case LOW:
      lcd.noBacklight();
      break;
  }
}

void display_main() {
  lcd.clear();

  // prima riga
  lcd.home();
  lcd.print("Air ");
  lcd.print(air.hum);
  lcd.print("% ");
  lcd.setCursor(10, 0);
  lcd.print(air.temp);
  lcd.print("\xDF"
            "C");

  lcd.setCursor(0, 1);  // va a capo

  //seconda riga
  String g = geo.STATUS == COLD ? "COLD" : (geo.STATUS == HOT ? "HOT" : "---");
  String m = moisture.STATUS == DRY ? "DRY" : (moisture.STATUS == WET ? "WET" : "---");
  m = soil.waterLock ? "LOCK " : m;
  lcd.print(g);
  lcd.print(" ");
  lcd.print(m);
  lcd.setCursor(10, 1);
  lcd.print(int(geo.temp));
  lcd.print("\xDF"
            "C");
}

void display_thresholds() {
  lcd.clear();
  lcd.home();

  lcd.print("Lux  Moist  Heat");
  lcd.setCursor(0, 1);  // va a capo

  lcd.print(LUMEN_THRESHOLD);
  lcd.setCursor(6, 1);
  lcd.print(SOIL_HUM_THRESHOLD);
  lcd.setCursor(12, 1);
  lcd.print(SOIL_TEMP_THRESHOLD);
}

void display_moisture() {
  lcd.clear();
  lcd.home();

  lcd.print("MOISTURE ");
  lcd.print(moisture.value);

  lcd.setCursor(0, 1);  // va a capo

  lcd.print("Threshod ");
  lcd.print(SOIL_HUM_THRESHOLD);
}

void display_lux() {
  lcd.clear();
  lcd.home();

  lcd.print("LUMEN ");
  if (lumen.AUTO_MODE) {
    lcd.print("AUTO ");
  } else {
    lcd.print("MAN ");
  }

  lcd.print(lumen.value);

  lcd.setCursor(0, 1);  // va a capo

  lcd.print("Threshod ");
  lcd.print(LUMEN_THRESHOLD);
}


void display_heat() {
  lcd.clear();
  lcd.home();

  lcd.print("HEAT ");
  lcd.print(geo.temp);
  // lcd.print(map(geo.temp, 0, 35, 0, 1024));

  lcd.setCursor(0, 1);  // va a capo

  lcd.print("Threshod ");
  lcd.print(map(SOIL_TEMP_THRESHOLD, 0, 1024, 0, 35));
}
