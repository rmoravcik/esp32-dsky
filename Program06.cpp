#include "Program06.h"

static Program06 *inst = NULL;

Program06::Program06(DSKY *dsky)
{
  m_dsky = dsky;
}

Program06::~Program06()
{
}

uint8_t program06_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Program06(dsky);
  }

  Serial.print("PROGRAM06 started at ");
  Serial.println(millis());

  inst->m_actyCounter = 0;
  inst->m_dsky->di->setVerbCode("50");
  inst->m_dsky->di->setNounCode("25");
  inst->m_dsky->di->setRegister1("+00062");
  inst->m_dsky->di->setRegister2("+00000");
  inst->m_dsky->di->setRegister3("+00000");
  inst->m_dsky->di->setVerbCodeBlinking(true);
  inst->m_dsky->di->setNounCodeBlinking(true);
  return DSKY_STATE_BUSY;
}

uint8_t program06_cycle(char key, bool stopRequested, uint8_t state)
{
  if (stopRequested) {
    Serial.print("PROGRAM06 finished at ");
    Serial.println(millis());
    return DSKY_STATE_IDLE;
  }

  if (key == KEY_PRO) {
    if (!inst->m_dsky->standbyMode) {
      inst->m_dsky->di->powerDownIndicator();
      inst->m_dsky->ai->powerDownIndicator();
      ledcWrite(0, 10);
      ledcWrite(1, 0);
      inst->m_dsky->standbyMode = true;
    } else {
      ledcWrite(0, 200);
      ledcWrite(1, 255);
      inst->m_dsky->ai->resetIndicator();
      inst->m_dsky->di->resetIndicator();
      inst->m_dsky->standbyMode = false;
      return DSKY_STATE_INIT;
    }
  }

  if (!inst->m_dsky->standbyMode) {
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
  }

  return state;
}
