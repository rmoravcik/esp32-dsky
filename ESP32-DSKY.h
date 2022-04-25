#ifndef ESP32_DSKY_H
#define ESP32_DSKY_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

#define MAIN_LOOP_DELAY_MS  (10)
#define ACTY_ON_DELAY_MS    (500 / MAIN_LOOP_DELAY_MS)
#define ACTY_OFF_DELAY_MS   (1500 / MAIN_LOOP_DELAY_MS)

#define WEATHER_UPDATE_S    (15 * 60)

#define FAGC_INIT           (0)
#define FAGC_IDLE           (1)
#define FAGC_BUSY           (2)

#define VERB_CODE_INVALID   (-1)
#define NOUN_CODE_INVALID   (-1)

typedef uint8_t (*startFn_t)(AlarmIndicator*, DigitalIndicator*);
typedef uint8_t (*cycleFn_t)(void);

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

#endif /* ESP32_DSKY_h */
