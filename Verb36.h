#ifndef VERB36_H
#define VERB36_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

class Verb36
{
public:
  Verb36(AlarmIndicator *ai, DigitalIndicator *di);
  virtual ~Verb36();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;
};

uint8_t verb36_start(AlarmIndicator *ai, DigitalIndicator *di);
uint8_t verb36_cycle(void);

#endif /* VERB36_H */