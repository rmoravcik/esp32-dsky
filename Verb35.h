#ifndef VERB35_H
#define VERB35_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Verb35
{
public:
  Verb35(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Verb35();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  unsigned long startMillis;
};

uint8_t verb35_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb35_cycle(bool stop);

#endif /* VERB35_H */
