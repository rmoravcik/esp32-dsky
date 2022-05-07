#include <WiFi.h>
#include <TimeLib.h>

#include "ESP32-DSKY.h"
#include "Verb05.h"

static Verb05 *inst = NULL;

Verb05::Verb05(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
  m_weather = weather;
}

Verb05::~Verb05()
{
}

uint8_t verb05noun09_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb05(ai, di, weather);
  }

  inst->m_di->setRegister1("+00000");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3("+00000");

  Serial.print("VERB05NOUN09 started at ");
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

uint8_t verb05noun09_cycle(char key, bool stopRequested, uint8_t state)
{
  if (WiFi.status() != WL_CONNECTED) {
    _setErrorEmptyRegister("+01105");
  }

  Serial.print("VERB05NOUN09 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}
