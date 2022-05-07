#ifndef VERB05_H
#define VERB05_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Verb05
{
public:
  Verb05(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Verb05();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;
  Weather *m_weather;
};

uint8_t verb05noun09_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb05noun09_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB05_H */
