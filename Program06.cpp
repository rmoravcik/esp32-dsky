#include "ESP32-DSKY.h"
#include "Kbd.h"
#include "Program06.h"

static Program06 *inst = NULL;

Program06::Program06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
  
  m_standbyMode = false;
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
  inst->m_di->setVerbCode("50");
  inst->m_di->setNounCode("25");
  inst->m_di->setRegister1("+00062");
  inst->m_di->setRegister2("+00000");
  inst->m_di->setRegister3("+00000");
  inst->m_di->setVerbCodeBlinking(true);
  inst->m_di->setNounCodeBlinking(true);
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
    if (!inst->m_standbyMode) {
      inst->m_di->powerDownIndicator();
      inst->m_ai->powerDownIndicator();
      inst->m_standbyMode = true;
    } else {
      inst->m_ai->resetIndicator(false);
      inst->m_di->resetIndicator(false);
      inst->m_standbyMode = false;
      return DSKY_STATE_INIT;
    }
  }

  if (!inst->m_standbyMode) {
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
  }

  return state;
}
