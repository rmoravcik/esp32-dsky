#include <TimeLib.h>

#include "Verb24.h"

static Verb24 *inst = NULL;

Verb24::Verb24(DSKY *dsky)
{
  m_dsky = dsky;
}

Verb24::~Verb24()
{
}

bool Verb24::validateValue(String value, int32_t min, int32_t max)
{
  if (value.length() != 6) {
    Serial.print("Verb24::validateValue() value=");
    Serial.print(value);
    Serial.println(" to short!");
    return false;
  }

  if ((value[0] != '+') && (value[0] != '-')) {
    Serial.print("Verb24::validateValue() value=");
    Serial.print(value);
    Serial.println(" missing sign character!");
    return false;
  }

  if (value.toInt() > max) {
    Serial.print("Verb24::validateValue() value=");
    Serial.print(value);
    Serial.print(" greater then max=");
    Serial.print(max);
    Serial.println("!");
    return false;
  }

  if (value.toInt() < min) {
    Serial.print("Verb24::validateValue() value=");
    Serial.print(value);
    Serial.print(" less then min=");
    Serial.print(min);
    Serial.println("!");
    return false;
  }

  return true;
}

uint8_t verb24noun34_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb24(dsky);
  }

  inst->m_dsky->di->setRegister1(DIGITAL_INDICATOR_VALUE_NAN);
  inst->m_dsky->di->setRegister2(DIGITAL_INDICATOR_VALUE_NAN);
  inst->m_dsky->di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);

  inst->m_registerIndex = 0;

  Serial.print("VERB24NOUN34 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb24noun34_cycle(char key, bool stopRequested, uint8_t state)
{
  if ((stopRequested) || (inst->m_registerIndex == 2)) {
    if (inst->m_registerIndex == 2) {
      char value[6];

      snprintf(value, sizeof(value), "%02ld:%02ld",
               inst->m_dsky->di->getRegister1().toInt(),
               inst->m_dsky->di->getRegister2().toInt());
      inst->m_dsky->conf->setValue("standby_start_time", value);
      inst->m_dsky->conf->writeConfig();
    }

    Serial.print("VERB24NOUN34 finished at ");
    Serial.println(millis());
    return DSKY_STATE_IDLE;
  }

  if (isdigit(key) || (key == '+') || (key == '-')) {
    if (inst->m_registerIndex == 0) {
      if (inst->m_dsky->di->getRegister1().length() < 6) {
        if (inst->m_dsky->di->getRegister1() == DIGITAL_INDICATOR_VALUE_NAN) {
          inst->m_dsky->di->setRegister1(String(key));
        } else {
          inst->m_dsky->di->setRegister1(inst->m_dsky->di->getRegister1() + String(key));
        }
      } else {
        Serial.println("VERB24NOUN34: Register1 is full");
        inst->m_dsky->ai->setOperatorErrorStatusBlinking();
      }
    } else if (inst->m_registerIndex == 1) {
      if (inst->m_dsky->di->getRegister2().length() < 6) {
        if (inst->m_dsky->di->getRegister2() == DIGITAL_INDICATOR_VALUE_NAN) {
          inst->m_dsky->di->setRegister2(String(key));
        } else {
          inst->m_dsky->di->setRegister2(inst->m_dsky->di->getRegister2() + String(key));
        }
      } else {
        Serial.println("VERB24NOUN34: Register2 is full");
        inst->m_dsky->ai->setOperatorErrorStatusBlinking();
      }
    }
  }

  if (key == KEY_CLR) {
    if (inst->m_registerIndex == 0) {
      inst->m_dsky->di->setRegister1(DIGITAL_INDICATOR_VALUE_NAN);
    } else if (inst->m_registerIndex == 1) {
      inst->m_dsky->di->setRegister2(DIGITAL_INDICATOR_VALUE_NAN);
    }
  }

  if (key == KEY_ENTR) {
    if (inst->m_registerIndex == 0) {
      if (!inst->validateValue(inst->m_dsky->di->getRegister1(), 0, 23)) {
        Serial.println("VERB24NOUN34: Invalid hours");
        inst->m_dsky->ai->setOperatorErrorStatusBlinking();
      } else {
        inst->m_registerIndex++;
      }
    } else if (inst->m_registerIndex == 1) {
      if (!inst->validateValue(inst->m_dsky->di->getRegister2(), 0, 59)) {
        Serial.println("VERB24NOUN34: Invalid minutes");
        inst->m_dsky->ai->setOperatorErrorStatusBlinking();
      } else {
        inst->m_registerIndex++;
      }
    }
  }

  return DSKY_STATE_BUSY;
}
