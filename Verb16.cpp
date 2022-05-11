#include <TimeLib.h>

#include "Verb16.h"

static Verb16 *inst = NULL;

Verb16::Verb16(DSKY *dsky)
{
  m_dsky = dsky;
  randomSeed(now());
}

Verb16::~Verb16()
{
}

uint8_t verb16noun36_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb16(dsky);
  }

  inst->m_dsky->di->setRegister1("+00000");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3("+00000");

  inst->m_prevSecond = 0;

  Serial.print("VERB16NOUN36 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb16noun36_cycle(char key, bool stopRequested, uint8_t state)
{
  time_t currTime = now();

  if (stopRequested) {
    Serial.print("VERB16NOUN36 finished at ");
    Serial.println(millis());
    return DSKY_STATE_IDLE;
  }

  if (second(currTime) != inst->m_prevSecond) {
    char value[7];

    snprintf(value, sizeof(value), "+%05d", hour(currTime));
    inst->m_dsky->di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", minute(currTime));
    inst->m_dsky->di->setRegister2(value);
    snprintf(value, sizeof(value), "+%05d", (int32_t)((second(currTime) * 100) + random(10)));
    inst->m_dsky->di->setRegister3(value);

    if (((second(currTime) % 3) == 0) && (inst->m_actyCounter == 0)) {
      inst->m_dsky->di->setComputerActivityStatus(true);
      inst->m_actyCounter = ACTY_ON_DELAY_MS;
    }

    inst->m_prevSecond = second(currTime);
  }

  if (inst->m_actyCounter == 0) {
    inst->m_dsky->di->setComputerActivityStatus(false);
  } else {
    inst->m_actyCounter--;
  }

  return DSKY_STATE_BUSY;
}
