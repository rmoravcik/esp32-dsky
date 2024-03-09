#ifndef ENVSENSOR_H
#define ENVSENSOR_H

#include <bsec.h>
#include <TimeLib.h>

#include "AlarmIndicator.h"

class EnvSensor
{
public:
  EnvSensor(AlarmIndicator *ai);
  virtual ~EnvSensor();

  int16_t getTemperature(void) { return m_temperature; };
  uint16_t getHumidity(void) { return m_humidity; };
  uint16_t getIAQ(void) { return m_iaq; };
  uint16_t getIAQAccuracy(void) { return m_iaqAccuracy; };
  uint16_t getTVOC(void) { return m_tvoc; };
  uint16_t getCO2(void) { return m_co2; };

  bool update(void);

private:
  Bsec m_iaqSensor;
  AlarmIndicator *m_ai;

  int16_t m_temperature;
  uint16_t m_humidity;
  uint16_t m_iaq;
  uint8_t m_iaqAccuracy;
  uint16_t m_tvoc;
  uint16_t m_co2;

  uint8_t m_bsecState[BSEC_MAX_STATE_BLOB_SIZE];

  time_t m_lastUpdateState;
  time_t m_lastUpdate;

  void loadState(void);
  void updateState(void);
};

#endif /* ENVSENSOR_H */
