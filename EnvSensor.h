#ifndef ENVSENSOR_H
#define ENVSENSOR_H

#include <SGP30.h>
#include <TimeLib.h>

#include "AlarmIndicator.h"

class EnvSensor
{
public:
  EnvSensor(AlarmIndicator *ai);
  virtual ~EnvSensor();

  uint16_t getTVOC(void) { return m_tvoc; };
  uint16_t getCO2(void) { return m_co2; };

  bool update(void);

private:
  SGP30 m_sgp;
  AlarmIndicator *m_ai;

  uint16_t m_tvoc;
  uint16_t m_co2;

  time_t m_lastUpdate;
};

#endif /* ENVSENSOR_H */
