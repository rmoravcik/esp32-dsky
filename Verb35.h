#ifndef VERB35_H
#define VERB35_H

#include "ESP32-DSKY.h"

class Verb35
{
public:
  Verb35(DSKY *dsky);
  virtual ~Verb35();

  DSKY *m_dsky;

  unsigned long startMillis;
};

uint8_t verb35_start(DSKY *dsky);
uint8_t verb35_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB35_H */
