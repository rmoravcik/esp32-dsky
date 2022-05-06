#include <WiFi.h>
#include <TimeLib.h>

#include "ESP32-DSKY.h"
#include "Verb06.h"

static Verb06 *inst = NULL;

Verb06::Verb06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
  m_weather = weather;
}

Verb06::~Verb06()
{
}

uint8_t verb06noun09_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb06(ai, di, weather);
  }

  inst->m_di->setRegister1("+00000");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3("+00000");

  Serial.print("VERB06NOUN09 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

static void _setErrorEmptyRegister(String value)
{
  if (inst->m_di->getRegister1() == "+00000") {
    inst->m_di->setRegister1(value);
  } else if (inst->m_di->getRegister2() == "+00000") {
    inst->m_di->setRegister2(value);
  } else if (inst->m_di->getRegister3() == "+00000") {
    inst->m_di->setRegister3(value);
  }
}

uint8_t verb06noun09_cycle(char key, bool stopRequested, uint8_t state)
{
  if (WiFi.status() != WL_CONNECTED) {
    _setErrorEmptyRegister("+01106");
  }

  Serial.print("VERB06NOUN09 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}

uint8_t verb06noun43_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb06(ai, di, weather);
  }

  inst->m_di->setRegister1("+00000");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);

  Serial.print("VERB06NOUN43 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb06noun43_cycle(char key, bool stopRequested, uint8_t state)
{
  char value[7];

  if (inst->m_weather) {
    snprintf(value, sizeof(value), "+%05d", inst->m_weather->getLatitude());
    inst->m_di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_weather->getLongitude());
    inst->m_di->setRegister2(value);
  }

  Serial.print("VERB06NOUN43 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}

uint8_t verb06noun95_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb06(ai, di, weather);
  }

  inst->m_di->setRegister1("+00000");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3("+00000");

  Serial.print("VERB06NOUN95 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb06noun95_cycle(char key, bool stopRequested, uint8_t state)
{
  char value[7];

  if (inst->m_weather) {
    uint32_t absTemperatureValue = abs(inst->m_weather->getTemperature());
    if (inst->m_weather->getTemperature() >= 0) {
      snprintf(value, sizeof(value), "+%05d", absTemperatureValue);
    } else {
      snprintf(value, sizeof(value), "-%05d", absTemperatureValue);
    }
    inst->m_di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_weather->getPressure());
    inst->m_di->setRegister2(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_weather->getHumidity());
    inst->m_di->setRegister3(value);
  }

  Serial.print("VERB06NOUN95 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}
