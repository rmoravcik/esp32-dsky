#include "ESP32-DSKY.h"
#include "Verb35.h"

Verb35 *inst = NULL;

Verb35::Verb35(AlarmIndicator *ai, DigitalIndicator *di)
{
  m_ai = ai;
  m_di = di;
  
  m_ai->setUplinkActivityStatus(false);
  m_ai->setNoAttitudeStatus(false);
  m_ai->setStandbyStatus(false);
  m_ai->setKeyReleaseStatus(false);
  m_ai->setOperatorErrorStatus(false);
  m_ai->setTemperatureCaution(false);
  m_ai->setGimbalLockStatus(false);
  m_ai->setProgramCondition(false);
  m_ai->setRestartCondition(false);
  m_ai->setTrackerCondition(false);
  m_ai->setAltitudeDataCaution(false);
  m_ai->setVelocityDataCaution(false);
  
  m_di->setProgramNumber(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  m_di->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  m_di->setRegister1(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);
  m_di->setRegister2(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);
  m_di->setRegister3(DIGITAL_INDICATOR_REGISTER_VALUE_NAN);
  m_di->setComputerActivityStatus(false);
}

Verb35::~Verb35()
{
}

uint8_t verb35_start(AlarmIndicator *ai, DigitalIndicator *di)
{
  if (inst == NULL) {
    inst = new Verb35(ai, di);
  }

  inst->startMillis = millis();

  Serial.print("VERB35 started at ");
  Serial.println(inst->startMillis);

  return FAGC_BUSY;
}

uint8_t verb35_cycle(void)
{
  unsigned long deltaMillis = millis() - inst->startMillis;

  if (deltaMillis > 5900) {
    return FAGC_IDLE;
  } else if (deltaMillis > 5800) {
    inst->m_ai->setUplinkActivityStatus(false);
    inst->m_ai->setNoAttitudeStatus(false);
    inst->m_ai->setStandbyStatus(false);
    inst->m_ai->setKeyReleaseStatus(false);
    inst->m_ai->setOperatorErrorStatus(false);
    inst->m_ai->setTemperatureCaution(false);
    inst->m_ai->setGimbalLockStatus(false);
    inst->m_ai->setProgramCondition(false);
    inst->m_ai->setRestartCondition(false);
    inst->m_ai->setTrackerCondition(false);
    inst->m_ai->setAltitudeDataCaution(false);
    inst->m_ai->setVelocityDataCaution(false);
    inst->m_di->setProgramNumber(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  } else if (deltaMillis > 4800) {
    inst->m_ai->setKeyReleaseStatus(true);
    inst->m_ai->setOperatorErrorStatus(true);
    inst->m_di->setVerbCode(88);
    inst->m_di->setNounCode(88);
  } else if (deltaMillis > 4400) {
    inst->m_ai->setKeyReleaseStatus(false);
    inst->m_ai->setOperatorErrorStatus(false);
    inst->m_di->setVerbCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
    inst->m_di->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  } else if (deltaMillis > 3400) {
    inst->m_ai->setKeyReleaseStatus(true);
    inst->m_ai->setOperatorErrorStatus(true);
    inst->m_di->setVerbCode(88);
    inst->m_di->setNounCode(88);
  } else if (deltaMillis > 3000) {
    inst->m_ai->setKeyReleaseStatus(false);
    inst->m_ai->setOperatorErrorStatus(false);
    inst->m_di->setVerbCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
    inst->m_di->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  } else if (deltaMillis > 2000) {
    inst->m_ai->setKeyReleaseStatus(true);
    inst->m_ai->setOperatorErrorStatus(true);
    inst->m_di->setVerbCode(88);
    inst->m_di->setNounCode(88);
  } else if (deltaMillis > 1600) {
    inst->m_ai->setKeyReleaseStatus(false);
    inst->m_ai->setOperatorErrorStatus(false);
    inst->m_di->setVerbCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
    inst->m_di->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
  } else if (deltaMillis > 600) {
    inst->m_ai->setKeyReleaseStatus(true);
    inst->m_ai->setOperatorErrorStatus(true);
    inst->m_di->setVerbCode(88);
    inst->m_di->setNounCode(88);
  } else if (deltaMillis > 500) {
    inst->m_di->setRegister3(88888);
    inst->m_di->setProgramNumber(88);
  } else if (deltaMillis > 400) {
    inst->m_di->setRegister2(88888);
  } else if (deltaMillis > 300) {
    inst->m_ai->setKeyReleaseStatus(false);
    inst->m_ai->setOperatorErrorStatus(false);
    inst->m_di->setVerbCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
    inst->m_di->setNounCode(DIGITAL_INDICATOR_VALUE_UINT8_NAN);
    inst->m_di->setRegister1(88888);
  } else if (deltaMillis > 200) {
    inst->m_di->setNounCode(88);
  } else if (deltaMillis > 100) {
    inst->m_ai->setUplinkActivityStatus(true);
    inst->m_ai->setNoAttitudeStatus(true);
    inst->m_ai->setStandbyStatus(true);
    inst->m_ai->setKeyReleaseStatus(true);
    inst->m_ai->setOperatorErrorStatus(true);
    inst->m_ai->setTemperatureCaution(true);
    inst->m_ai->setGimbalLockStatus(true);
    inst->m_ai->setProgramCondition(true);
    inst->m_ai->setRestartCondition(true);
    inst->m_ai->setTrackerCondition(true);
    inst->m_ai->setAltitudeDataCaution(true);
    inst->m_ai->setVelocityDataCaution(true);
    inst->m_di->setVerbCode(88);
  }
  return FAGC_BUSY;
}
