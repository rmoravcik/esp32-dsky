#ifndef VERB24_H
#define VERB24_H

#include "ESP32-DSKY.h"

class Verb24
{
public:
  Verb24(DSKY *dsky);
  virtual ~Verb24();

  bool validateValue(String value, int32_t min, int32_t max);

  DSKY *m_dsky;

  uint8_t m_registerIndex;
};

uint8_t verb24noun34_start(DSKY *dsky);
uint8_t verb24noun34_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB24_H */
