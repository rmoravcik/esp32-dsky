#include "ESP32-DSKY.h"
#include "EnvSensor.h"

static EnvSensor *inst = NULL;

EnvSensor::EnvSensor(AlarmIndicator *ai)
{
  inst = this;

  m_ai = ai;
  m_tvoc = 0;
  m_co2 = 0;

  m_sgp.GenericReset();
}

EnvSensor::~EnvSensor()
{
}

bool EnvSensor::update(void)
{
  if ((now() - m_lastUpdate) > ENV_SENSOR_UPDATE_S) {
    m_lastUpdate = now();
    m_sgp.request();
  }

  if (m_sgp.read()) {
    m_tvoc = m_sgp.getTVOC();
    m_co2 = m_sgp.getCO2();

    if ((m_tvoc > TVOC_LEVEL_POOR) || (m_co2 > CO2_LEVEL_POOR))
    {
      m_ai->setVelocityDataCaution(true);
    }

    return true;
  }
  
  return false;
}
