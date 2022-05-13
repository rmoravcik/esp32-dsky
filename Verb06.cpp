#include <TimeLib.h>

#include "Verb06.h"

static Verb06 *inst = NULL;

Verb06::Verb06(DSKY *dsky)
{
  m_dsky = dsky;
}

Verb06::~Verb06()
{
}

uint8_t verb06noun34_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb06(dsky);
  }

  inst->m_dsky->di->setRegister1("+00000");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);

  Serial.print("VERB06NOUN34 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb06noun34_cycle(char key, bool stopRequested, uint8_t state)
{
  char value[7];

  uint8_t hours, minutes;
  inst->m_dsky->rtc->convertTime(inst->m_dsky->conf->getString("standby_start_time"), &hours, &minutes);

  snprintf(value, sizeof(value), "+%05u", hours);
  inst->m_dsky->di->setRegister1(value);
  snprintf(value, sizeof(value), "+%05u", minutes);
  inst->m_dsky->di->setRegister2(value);

  Serial.print("VERB06NOUN34 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}

uint8_t verb06noun43_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb06(dsky);
  }

  inst->m_dsky->di->setRegister1("+00000");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);

  Serial.print("VERB06NOUN43 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb06noun43_cycle(char key, bool stopRequested, uint8_t state)
{
  char value[7];

  if (inst->m_dsky->weather) {
    snprintf(value, sizeof(value), "+%05d", inst->m_dsky->weather->getLatitude());
    inst->m_dsky->di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_dsky->weather->getLongitude());
    inst->m_dsky->di->setRegister2(value);
  }

  Serial.print("VERB06NOUN43 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}

uint8_t verb06noun95_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb06(dsky);
  }

  inst->m_dsky->di->setRegister1("+00000");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3("+00000");

  Serial.print("VERB06NOUN95 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb06noun95_cycle(char key, bool stopRequested, uint8_t state)
{
  char value[7];

  if (inst->m_dsky->weather) {
    uint32_t absTemperatureValue = abs(inst->m_dsky->weather->getTemperature());
    if (inst->m_dsky->weather->getTemperature() >= 0) {
      snprintf(value, sizeof(value), "+%05d", absTemperatureValue);
    } else {
      snprintf(value, sizeof(value), "-%05d", absTemperatureValue);
    }
    inst->m_dsky->di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_dsky->weather->getPressure());
    inst->m_dsky->di->setRegister2(value);
    snprintf(value, sizeof(value), "+%05d", inst->m_dsky->weather->getHumidity());
    inst->m_dsky->di->setRegister3(value);
  }

  Serial.print("VERB06NOUN95 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}
