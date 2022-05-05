#include "ESP32-DSKY.h"
#include "Kbd.h"
#include "Program06.h"

static Program06 *inst = NULL;

Program06::Program06(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
  
  m_standby = false;
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
  return FAGC_BUSY;
}

uint8_t program06_cycle(char key, bool stop)
{
  if (stop) {
    Serial.print("PROGRAM06 finished at ");
    Serial.println(millis());
    return FAGC_IDLE;
  }

  if (key = KEY_PRO) {
    if (!inst->m_standby) {
      inst->m_di->powerDownIndicator();
      inst->m_ai->powerDownIndicator();
      inst->m_standby = true;
    } else {
      inst->m_standby = false;
    }
  }

  if (!inst->m_standby) {
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

  return FAGC_BUSY;
}
