#ifndef PROGRAM06_H
#define PROGRAM06_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Program06
{
public:
  Program06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Program06();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  uint8_t m_actyCounter;
  bool m_standbyMode;
};

uint8_t program06_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t program06_cycle(char key, bool stopRequested, uint8_t state);

#endif /* PROGRAM06_H */
