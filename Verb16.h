#ifndef VERB16_H
#define VERB16_H

#include "ESP32-DSKY.h"

class Verb16
{
public:
  Verb16(DSKY *dsky);
  virtual ~Verb16();

  DSKY *m_dsky;

  uint8_t m_prevSecond;
  uint8_t m_actyCounter;
};

uint8_t verb16noun36_start(DSKY *dsky);
uint8_t verb16noun36_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB16_H */
