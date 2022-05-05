#ifndef ESP32_DSKY_H
#define ESP32_DSKY_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

#define MAIN_LOOP_DELAY_MS     (10)

#define ACTY_ON_DELAY_MS       (500 / MAIN_LOOP_DELAY_MS)
#define ACTY_OFF_DELAY_MS      (1500 / MAIN_LOOP_DELAY_MS)

#define VERB_NOUN_OFF_DELAY_MS (400 / MAIN_LOOP_DELAY_MS)
#define VERB_NOUN_ON_DELAY_MS  (1000 / MAIN_LOOP_DELAY_MS)

#define WEATHER_UPDATE_S       (15 * 60)

#define DSKY_STATE_INIT           (0)
#define DSKY_STATE_IDLE           (1)
#define DSKY_STATE_BUSY           (2)

#define VERB_CODE_INVALID        (-1)
#define NOUN_CODE_INVALID        (-1)
#define PROGRAM_NUMBER_INVALID   (-1)

typedef uint8_t (*startFn_t)(AlarmIndicator*, DigitalIndicator*, Weather*);
typedef uint8_t (*cycleFn_t)(char key, bool stopRequested, uint8_t state);

struct noun {
  int8_t code;
  startFn_t startFn;
  cycleFn_t cycleFn;
};

struct verb {
  int8_t code;
  bool nounRequired;
  startFn_t startFn;
  cycleFn_t cycleFn;
  struct noun* nouns;
};

struct program {
  int8_t number;
  startFn_t startFn;
  cycleFn_t cycleFn;
};

#endif /* ESP32_DSKY_h */
