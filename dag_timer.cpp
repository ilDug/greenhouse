#include "dag_timer.h"

void dagtimerNoop() {} // funzione segnaposto che non fa niente// esegue periodo del timer e le call back

DagTimer::DagTimer(unsigned long time_duration) {
  duration = time_duration;
  FIRED = 0;
  LOOP = true;
  call_back = dagtimerNoop;
}

void DagTimer::init() {
  init(true);
}
void DagTimer::init(boolean repeat) {
  bookmark = millis();
  FIRED = 0;
  LOOP = repeat;
}

void DagTimer::run(void (*fun)(void)) {
  if (triggered()) {
    call_back = fun;
    call_back();
  }
}

bool DagTimer::triggered() {
  unsigned long dt = (millis() - bookmark);
  boolean res = false;

  if ((dt > duration) && !FIRED) {
    FIRED = 1;
    res = true;

    if (LOOP) {
      bookmark = millis();
      FIRED = 0;
    }
  } else res = false;

  return res;
}

bool DagTimer::clock(){
  return triggered();
}



