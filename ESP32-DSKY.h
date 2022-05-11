#ifndef ESP32_DSKY_H
#define ESP32_DSKY_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Kbd.h"
#include "OTA.h"
#include "RTC.h"
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

#define GPIO_TFT_RST              (0)
#define GPIO_ALARM_INDICATOR_CS   (5)
#define GPIO_BACKLIGHT            (13)
#define GPIO_DIGITAL_INDICATOR_CS (26)

struct DSKY {
  AlarmIndicator *ai;
  DigitalIndicator *di;
  Kbd *kbd;
  OTA *ota;
  RTC *rtc;
  Weather *weather;
};

typedef uint8_t (*startFn_t)(DSKY *);
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
