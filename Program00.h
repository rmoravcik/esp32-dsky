#ifndef PROGRAM00_H
#define PROGRAM00_H

#include "ESP32-DSKY.h"

class Program00
{
public:
  Program00(DSKY *dsky);
  virtual ~Program00();

  DSKY *m_dsky;

  uint8_t m_actyCounter;
};

uint8_t program00_start(DSKY *dsky);
uint8_t program00_cycle(char key, bool stopRequested, uint8_t state);

#endif /* PROGRAM00_H */
