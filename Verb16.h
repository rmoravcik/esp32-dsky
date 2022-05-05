#ifndef VERB16_H
#define VERB16_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

class Verb16
{
public:
  Verb16(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Verb16();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  uint8_t m_prevSecond;
  uint8_t m_actyCounter;
};

uint8_t verb16noun36_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb16noun36_cycle(char key, bool stop);

#endif /* VERB16_H */
