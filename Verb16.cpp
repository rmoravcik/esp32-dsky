#include <TimeLib.h>

#include "ESP32-DSKY.h"
#include "Verb16.h"

static Verb16 *inst = NULL;

Verb16::Verb16(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
  randomSeed(now());
}

Verb16::~Verb16()
{
}

uint8_t verb16noun36_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb16(ai, di, weather);
  }

  inst->m_di->setRegister1("+00000");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3("+00000");

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
    inst->m_di->setRegister1(value);
    snprintf(value, sizeof(value), "+%05d", minute(currTime));
    inst->m_di->setRegister2(value);
    snprintf(value, sizeof(value), "+%05d", (int32_t)((second(currTime) * 100) + random(10)));
    inst->m_di->setRegister3(value);

    if (((second(currTime) % 3) == 0) && (inst->m_actyCounter == 0)) {
      inst->m_di->setComputerActivityStatus(true);
      inst->m_actyCounter = ACTY_ON_DELAY_MS;
    }

    inst->m_prevSecond = second(currTime);
  }

  if (inst->m_actyCounter == 0) {
    inst->m_di->setComputerActivityStatus(false);
  } else {
    inst->m_actyCounter--;
  }

  return DSKY_STATE_BUSY;
}
