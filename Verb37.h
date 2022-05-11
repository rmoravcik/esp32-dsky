#ifndef VERB37_H
#define VERB37_H

#include "ESP32-DSKY.h"

extern cycleFn_t programCycleFn;

class Verb37
{
public:
  Verb37(DSKY *dsky);
  virtual ~Verb37();

  DSKY *m_dsky;
};

uint8_t verb37noun00_start(DSKY *dsky);
uint8_t verb37noun00_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb37noun06_start(DSKY *dsky);
uint8_t verb37noun06_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB37_H */
