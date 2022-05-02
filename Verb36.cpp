#include "ESP32-DSKY.h"
#include "Verb36.h"

static Verb36 *inst = NULL;

Verb36::Verb36(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
}

Verb36::~Verb36()
{
}

uint8_t verb36_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb36(ai, di, weather);
  }

  Serial.print("VERB36 started at ");
  Serial.println(millis());

  return FAGC_BUSY;
}

uint8_t verb36_cycle(bool stop)
{
  inst->m_ai->resetIndicator(false);
  inst->m_di->resetIndicator(false);
  return FAGC_INIT;
}
