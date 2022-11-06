#ifndef VERB06_H
#define VERB06_H

#include "ESP32-DSKY.h"

class Verb06
{
public:
  Verb06(DSKY *dsky);
  virtual ~Verb06();

  DSKY *m_dsky;
};

uint8_t verb06noun34_start(DSKY *dsky);
uint8_t verb06noun34_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun43_start(DSKY *dsky);
uint8_t verb06noun43_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun95_start(DSKY *dsky);
uint8_t verb06noun95_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun96_start(DSKY *dsky);
uint8_t verb06noun96_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun99_start(DSKY *dsky);
uint8_t verb06noun99_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB06_H */
