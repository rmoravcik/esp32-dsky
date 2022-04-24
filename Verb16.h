#ifndef VERB16_H
#define VERB16_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

class Verb16
{
public:
  Verb16(AlarmIndicator *ai, DigitalIndicator *di);
  virtual ~Verb16();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;

  unsigned long startMillis;
};

uint8_t verb16noun36_start(AlarmIndicator *ai, DigitalIndicator *di);
uint8_t verb16noun36_cycle(void);

#endif /* VERB16_H */
