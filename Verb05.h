#ifndef VERB05_H
#define VERB05_H

#include "ESP32-DSKY.h"

class Verb05
{
public:
  Verb05(DSKY *dsky);
  virtual ~Verb05();

  DSKY *m_dsky;
};

uint8_t verb05noun09_start(DSKY *dsky);
uint8_t verb05noun09_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB05_H */
