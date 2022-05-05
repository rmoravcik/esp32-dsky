#ifndef PROGRAM00_H
#define PROGRAM00_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Program00
{
public:
  Program00(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Program00();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  uint8_t m_actyCounter;
};

uint8_t program00_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t program00_cycle(char key, bool stop);

#endif /* PROGRAM00_H */
