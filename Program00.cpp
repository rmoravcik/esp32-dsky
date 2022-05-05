#include "ESP32-DSKY.h"
#include "Program00.h"

static Program00 *inst = NULL;

Program00::Program00(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
}

Program00::~Program00()
{
}

uint8_t program00_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Program00(ai, di, weather);
  }

  Serial.print("PROGRAM00 started at ");
  Serial.println(millis());

  inst->m_actyCounter = 0;
  return DSKY_STATE_BUSY;
}

uint8_t program00_cycle(char key, bool stopRequested, uint8_t state)
{
  if (stopRequested) {
    Serial.print("PROGRAM00 finished at ");
    Serial.println(millis());
    return state;
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
  return state;
}
