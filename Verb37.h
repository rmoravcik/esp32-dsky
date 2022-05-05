#ifndef VERB37_H
#define VERB37_H

#include "ESP32-DSKY.h"
#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

extern cycleFn_t programCycleFn;

class Verb37
{
public:
  Verb37(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
  virtual ~Verb37();

  AlarmIndicator *m_ai;
  DigitalIndicator *m_di;
};

uint8_t verb37noun00_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb37noun00_cycle(char key, bool stop);

uint8_t verb37noun06_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb37noun06_cycle(char key, bool stop);

#endif /* VERB37_H */
