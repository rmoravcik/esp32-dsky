#ifndef VERB06_H
#define VERB06_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Verb06
{
public:
  Verb06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Verb06();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;
  Weather *m_weather;
};

uint8_t verb06noun43_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb06noun43_cycle(bool stop);

uint8_t verb06noun95_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb06noun95_cycle(bool stop);

#endif /* VERB06_H */
