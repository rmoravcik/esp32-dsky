#ifndef ESP32_DSKY_H
#define ESP32_DSKY_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

#define MAIN_LOOP_DELAY_MS  (10)
#define ACTY_ON_DELAY_MS    (500 / MAIN_LOOP_DELAY_MS)
#define ACTY_OFF_DELAY_MS   (1500 / MAIN_LOOP_DELAY_MS)

#define FAGC_INIT           (0)
#define FAGC_IDLE           (1)
#define FAGC_BUSY           (2)

enum VERB_LIST {
  VERB35 = 0,
  VERB69,
  VERB_COUNT,
  VERB_UNSET
};

struct verb {
  uint8_t (*startFn)(AlarmIndicator*, DigitalIndicator*);
  uint8_t (*cycleFn)(void);
};

#endif /* ESP32_DSKY_h */
