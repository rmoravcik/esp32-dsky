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

uint8_t verb06noun09_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb06noun09_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun43_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb06noun43_cycle(char key, bool stopRequested, uint8_t state);

uint8_t verb06noun95_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb06noun95_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB06_H */
