#include "Program00.h"

static Program00 *inst = NULL;

Program00::Program00(DSKY *dsky)
{
  m_dsky = dsky;
}

Program00::~Program00()
{
}

uint8_t program00_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Program00(dsky);
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

  bool acty = inst->m_dsky->di->getComputerActivityStatus();
  if (!acty) {
    if (inst->m_actyCounter > ACTY_OFF_DELAY_MS) {
      inst->m_actyCounter = 0;
      inst->m_dsky->di->setComputerActivityStatus(true);
    }
  } else {
    if (inst->m_actyCounter > ACTY_ON_DELAY_MS) {
      inst->m_actyCounter = 0;
      inst->m_dsky->di->setComputerActivityStatus(false);
    }
  }

  inst->m_actyCounter++;
  return state;
}
