#include "ESP32-DSKY.h"
#include "Program06.h"

static Program06 *inst = NULL;

Program06::Program06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
}

Program06::~Program06()
{
}

uint8_t program06_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Program06(ai, di, weather);
  }

  Serial.print("PROGRAM06 started at ");
  Serial.println(millis());

  inst->m_actyCounter = 0;
  return FAGC_BUSY;
}

uint8_t program06_cycle(bool stop)
{
  if (stop) {
    Serial.print("PROGRAM06 finished at ");
    Serial.println(millis());
    return FAGC_IDLE;
  }

  bool acty = inst->m_di->getComputerActivityStatus();
  if (!acty) {
    if (inst->m_actyCounter > ACTY_OFF_DELAY_MS) {
      inst->m_actyCounter = 0;
      inst->m_di->setComputerActivityStatus(true);
    }
  } else {
    if (inst->m_actyCounter > ACTY_ON_DELAY_MS) {
      inst->m_actyCounter = 0;
      inst->m_di->setComputerActivityStatus(false);
    }
  }

  inst->m_actyCounter++;
  return FAGC_BUSY;
}
