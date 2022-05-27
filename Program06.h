#ifndef PROGRAM06_H
#define PROGRAM06_H

#include "ESP32-DSKY.h"

#define PRO_KEY_HOLD_COUNTER_MS       (2000 / MAIN_LOOP_DELAY_MS)

class Program06
{
public:
  Program06(DSKY *dsky);
  virtual ~Program06();

  DSKY *m_dsky;

  uint8_t m_actyCounter;

  char m_key;
  KeyState m_keyState;
  uint16_t m_holdCounter;
};

uint8_t program06_start(DSKY *dsky);
uint8_t program06_cycle(char key, bool stopRequested, uint8_t state);

#endif /* PROGRAM06_H */
