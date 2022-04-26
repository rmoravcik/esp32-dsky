#include <TimeLib.h>

#include "ESP32-DSKY.h"
#include "Verb16.h"

static Verb16 *inst = NULL;

Verb16::Verb16(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
}

Verb16::~Verb16()
{
}

uint8_t verb16noun36_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb16(ai, di, weather);
  }

  inst->m_di->setRegister1(0);
  inst->m_di->setRegister2(0);
  inst->m_di->setRegister3(0);

  inst->m_prevSecond = 0;

  Serial.print("VERB16NOUN36 started at ");
  Serial.println(millis());

  return FAGC_BUSY;
}

uint8_t verb16noun36_cycle(bool stop)
{
  time_t currTime = now();

  if (stop) {
    Serial.print("VERB16NOUN36 finished at ");
    Serial.println(millis());
    return FAGC_IDLE;
  }

  if (second(currTime) != inst->m_prevSecond) {
    inst->m_di->setRegister1(hour(currTime));
    inst->m_di->setRegister2(minute(currTime));
    inst->m_di->setRegister3(second(currTime) * 1000);

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

  return FAGC_BUSY;
}
