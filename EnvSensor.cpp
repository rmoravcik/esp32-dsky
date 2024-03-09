#include <EEPROM.h>
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

  memset(m_bsecState, 0, BSEC_MAX_STATE_BLOB_SIZE);

  m_lastUpdateState = 0;
  m_lastUpdate = 0;

  EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1);

  m_iaqSensor.begin((uint8_t)GPIO_BME680_CS, spi);

  if (m_iaqSensor.bme68xStatus != BME68X_OK) {
    Serial.print("ERROR: BME680 not detected!");
    return;  
  }

  m_iaqSensor.setConfig(bsec_config_iaq);

  loadState();

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
  if (m_iaqSensor.bme68xStatus != BME68X_OK) {
    return false;
  }

  if ((now() - m_lastUpdate) > ENV_SENSOR_UPDATE_S) {
    m_lastUpdate = now();

    if (m_iaqSensor.run()) {
      m_temperature = m_iaqSensor.temperature * 10;
      m_humidity = m_iaqSensor.humidity;
      m_iaq = m_iaqSensor.staticIaq;
      m_iaqAccuracy = m_iaqSensor.staticIaqAccuracy;
      m_tvoc = m_iaqSensor.breathVocEquivalent;
      m_co2 = m_iaqSensor.co2Equivalent;

#if DEBUG
      Serial.println("Air quality:");
      Serial.print(" Temperature: ");
      Serial.println(m_temperature);
      Serial.print(" Humidity: ");
      Serial.println(m_humidity);
      Serial.print(" IAQ: ");
      Serial.print(m_iaq);
      Serial.print(" Accuracy: ");
      Serial.println(m_iaqAccuracy);
      Serial.print(" TVOC: ");
      Serial.print(m_tvoc);
      Serial.print(" Accuracy: ");
      Serial.println(m_iaqSensor.breathVocAccuracy);
      Serial.print(" CO2: ");
      Serial.print(m_co2);
      Serial.print(" Accuracy: ");
      Serial.println(m_iaqSensor.co2Accuracy);
#endif

      if (m_iaq > IAQ_LEVEL_POLUTED) {
        m_ai->setVelocityDataCaution(true);
      }

      updateState();

      return true;
    }
  }

  return false;
}

void EnvSensor::loadState(void)
{
  if (EEPROM.read(0) == BSEC_MAX_STATE_BLOB_SIZE) {
    Serial.println("Reading BME680 state from EEPROM");

    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++) {
      m_bsecState[i] = EEPROM.read(i + 1);
    }
    
    m_iaqSensor.setState(m_bsecState);
  } else {
    Serial.println("Erasing BME680 state in EEPROM");

    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE + 1; i++) {
      EEPROM.write(i, 0);
    }

    EEPROM.commit();
  }
}

void EnvSensor::updateState(void)
{
  bool update = false;

  if (m_lastUpdateState == 0) {
    if (m_iaqSensor.iaqAccuracy >= 3) {
      update = true;
    }
  } else {
    if (m_iaqSensor.iaqAccuracy < 3) {
      m_lastUpdateState = 0;
      return;
    }

    if ((now() - m_lastUpdateState) > ENV_SENSOR_SAVE_S) {
      update = true;
    }
  }
  
  if (update) {
    m_lastUpdateState = now();
    m_iaqSensor.getState(m_bsecState);

    Serial.println("Writing BME860 state to EEPROM");
    
    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE ; i++) {
      EEPROM.write(i + 1, m_bsecState[i]);
    }
 
    EEPROM.write(0, BSEC_MAX_STATE_BLOB_SIZE);
    EEPROM.commit();
  }
}
