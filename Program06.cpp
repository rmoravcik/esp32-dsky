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

  inst->m_key = NO_KEY;
  inst->m_keyState = IDLE;

  return DSKY_STATE_BUSY;
}

uint8_t program06_cycle(char key, bool stopRequested, uint8_t state)
{
  if (stopRequested) {
    Serial.print("PROGRAM06 finished at ");
    Serial.println(millis());
    return DSKY_STATE_IDLE;
  }

  if (inst->m_dsky->kbd->getState() == PRESSED) {
      if (inst->m_keyState == IDLE) {
        inst->m_key = key;
        inst->m_keyState = PRESSED;
        Serial.print("IDLE->PRESSED key=");
        Serial.println(inst->m_key);
      }
  } else if (inst->m_dsky->kbd->getState() == HOLD) {
      if (inst->m_keyState == PRESSED) {
        inst->m_keyState = HOLD;
        Serial.print("PRESSED->HOLD key=");
        Serial.println(inst->m_key);
      }
  } else if (inst->m_dsky->kbd->getState() == RELEASED) {
      if (inst->m_keyState == HOLD) {
        inst->m_keyState = RELEASED;
        Serial.print("HOLD->RELEASED key=");
        Serial.println(inst->m_key);
      } else {
        inst->m_keyState = IDLE;
        inst->m_key = NO_KEY;
        Serial.println("RELEASED->IDLE");
      }
  }

  if (((inst->m_key == KEY_PRO) && (inst->m_keyState == RELEASED)) || (key == (KEY_PRO_FORCE))) {
    inst->m_key = NO_KEY;
    inst->m_keyState = IDLE;

    if (!inst->m_dsky->standbyMode) {
      inst->m_dsky->di->powerDownIndicator();
      inst->m_dsky->ai->powerDownIndicator();
      ledcWrite(0, 2);
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
