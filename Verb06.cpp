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

uint8_t verb06noun43_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb06(ai, di, weather);
  }

  inst->m_di->setRegister1(0);
  inst->m_di->setRegister2(0);
  inst->m_di->setRegister3(0);

  Serial.print("VERB06NOUN43 started at ");
  Serial.println(millis());

  return FAGC_BUSY;
}

uint8_t verb06noun43_cycle(void)
{
  inst->m_di->setRegister1(inst->m_weather->getLongitude());
  inst->m_di->setRegister2(inst->m_weather->getLatitude());

  return FAGC_IDLE;
}