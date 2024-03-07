#include "SPI.h"
#include "ESP32-DSKY.h"
#include "EnvSensor.h"

static EnvSensor *inst = NULL;
extern SPIClass spi;

const uint8_t bsec_config_iaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

EnvSensor::EnvSensor(AlarmIndicator *ai)
{
  inst = this;

  m_ai = ai;
  m_tvoc = 0;
  m_co2 = 0;

  m_lastUpdate = 0;

  m_iaqSensor.begin((uint8_t)GPIO_BME680_CS, spi);

  Serial.print("bme68xStatus=");
  Serial.println(m_iaqSensor.bme68xStatus);

  m_iaqSensor.setConfig(bsec_config_iaq);

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
  m_iaqSensor.setTemperatureOffset(10);
}

EnvSensor::~EnvSensor()
{
}

bool EnvSensor::update(void)
{
  if (m_iaqSensor.bme68xStatus != BME68X_OK)
  {
    return false;
  }

  if ((now() - m_lastUpdate) > ENV_SENSOR_UPDATE_S) {
    m_lastUpdate = now();

    if (m_iaqSensor.run()) {
      m_temperature = m_iaqSensor.temperature * 10;
      m_humidity = m_iaqSensor.humidity;
      m_iaq = m_iaqSensor.staticIaq;
      m_tvoc = m_iaqSensor.breathVocEquivalent;
      m_co2 = m_iaqSensor.co2Equivalent;

      Serial.print("temparature=");
      Serial.println(m_temperature);
      Serial.print("humidity=");
      Serial.println(m_humidity);
      Serial.print("iaq=");
      Serial.println(m_iaq);
      Serial.print("staticIaqAccuracy=");
      Serial.println(m_iaqSensor.staticIaqAccuracy);
      Serial.print("tvoc=");
      Serial.println(m_tvoc);
      Serial.print("breathVocAccuracy=");
      Serial.println(m_iaqSensor.breathVocAccuracy);
      Serial.print("co2=");
      Serial.println(m_co2);
      Serial.print("co2Accuracy=");
      Serial.println(m_iaqSensor.co2Accuracy);

      if ((m_tvoc > TVOC_LEVEL_POOR) || (m_co2 > CO2_LEVEL_POOR))
      {
        m_ai->setVelocityDataCaution(true);
      }

      return true;
    }
  }

  return false;
}
