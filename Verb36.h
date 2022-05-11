#ifndef VERB36_H
#define VERB36_H

#include "ESP32-DSKY.h"

class Verb36
{
public:
  Verb36(DSKY *dsky);
  virtual ~Verb36();

  DSKY *m_dsky;
};

uint8_t verb36_start(DSKY *dsky);
uint8_t verb36_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB36_H */
