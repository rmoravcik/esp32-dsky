#include "SPI.h"
#include "ESP32-DSKY.h"
#include "EnvSensor.h"

static EnvSensor *inst = NULL;
extern SPIClass spi;

EnvSensor::EnvSensor(AlarmIndicator *ai)
{
  inst = this;

  m_ai = ai;
  m_tvoc = 0;
  m_co2 = 0;

  m_iaqSensor.begin((uint8_t)GPIO_BME680_CS, spi);

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };
      
  m_iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
}

EnvSensor::~EnvSensor()
{
}

bool EnvSensor::update(void)
{
  if ((now() - m_lastUpdate) > ENV_SENSOR_UPDATE_S) {
    if (m_iaqSensor.run()) {
      m_temperature = m_iaqSensor.temperature * 10;
      m_humidity = m_iaqSensor.humidity;
      m_iaq = m_iaqSensor.staticIaq;
      m_tvoc = m_iaqSensor.breathVocEquivalent;
      m_co2 = m_iaqSensor.co2Equivalent;

      if ((m_tvoc > TVOC_LEVEL_POOR) || (m_co2 > CO2_LEVEL_POOR))
      {
        m_ai->setVelocityDataCaution(true);
      }

      return true;
    }

    m_lastUpdate = now();
  }

  return false;
}
