#include <WiFi.h>
#include <TimeLib.h>

#include "Verb05.h"

static Verb05 *inst = NULL;

Verb05::Verb05(DSKY *dsky)
{
  m_dsky = dsky;
}

Verb05::~Verb05()
{
}

uint8_t verb05noun09_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb05(dsky);
  }

  inst->m_dsky->di->setRegister1("+00000");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3("+00000");

  Serial.print("VERB05NOUN09 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

static void _setErrorEmptyRegister(String value)
{
  if (inst->m_dsky->di->getRegister1() == "+00000") {
    inst->m_dsky->di->setRegister1(value);
  } else if (inst->m_dsky->di->getRegister2() == "+00000") {
    inst->m_dsky->di->setRegister2(value);
  } else if (inst->m_dsky->di->getRegister3() == "+00000") {
    inst->m_dsky->di->setRegister3(value);
  }
}

uint8_t verb05noun09_cycle(char key, bool stopRequested, uint8_t state)
{
  if (WiFi.status() != WL_CONNECTED) {
    _setErrorEmptyRegister("+01105");
  }

  if (inst->m_dsky->rtc->updateFailed()) {
    _setErrorEmptyRegister("+01106");
  }

  if (inst->m_dsky->weather->updateFailed()) {
    _setErrorEmptyRegister("+01108");
  }

  Serial.print("VERB05NOUN09 finished at ");
  Serial.println(millis());

  return DSKY_STATE_IDLE;
}
