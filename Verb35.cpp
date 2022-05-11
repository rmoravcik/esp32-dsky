#include "Verb35.h"

static Verb35 *inst = NULL;

Verb35::Verb35(DSKY *dsky)
{
  m_dsky = dsky;
  
  m_dsky->ai->setUplinkActivityStatus(false);
  m_dsky->ai->setNoAttitudeStatus(false);
  m_dsky->ai->setStandbyStatus(false);
  m_dsky->ai->setKeyReleaseStatus(false);
  m_dsky->ai->setOperatorErrorStatus(false);
  m_dsky->ai->setTemperatureCaution(false);
  m_dsky->ai->setGimbalLockStatus(false);
  m_dsky->ai->setProgramCondition(false);
  m_dsky->ai->setRestartCondition(false);
  m_dsky->ai->setTrackerCondition(false);
  m_dsky->ai->setAltitudeDataCaution(false);
  m_dsky->ai->setVelocityDataCaution(false);
  
  m_dsky->di->setProgramNumber(DIGITAL_INDICATOR_VALUE_NAN);
  m_dsky->di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
  m_dsky->di->setRegister1(DIGITAL_INDICATOR_VALUE_NAN);
  m_dsky->di->setRegister2(DIGITAL_INDICATOR_VALUE_NAN);
  m_dsky->di->setRegister3(DIGITAL_INDICATOR_VALUE_NAN);
  m_dsky->di->setComputerActivityStatus(false);
}

Verb35::~Verb35()
{
}

uint8_t verb35_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb35(dsky);
  }

  inst->startMillis = millis();

  Serial.print("VERB35 started at ");
  Serial.println(inst->startMillis);

  return DSKY_STATE_BUSY;
}

uint8_t verb35_cycle(char key, bool stopRequested, uint8_t state)
{
  unsigned long deltaMillis = millis() - inst->startMillis;

  if (deltaMillis > 5900) {
    Serial.print("VERB35 finished at ");
    Serial.println(millis());
    return DSKY_STATE_IDLE;
  } else if (deltaMillis > 5800) {
    inst->m_dsky->ai->setUplinkActivityStatus(false);
    inst->m_dsky->ai->setNoAttitudeStatus(false);
    inst->m_dsky->ai->setStandbyStatus(false);
    inst->m_dsky->ai->setKeyReleaseStatus(false);
    inst->m_dsky->ai->setOperatorErrorStatus(false);
    inst->m_dsky->ai->setTemperatureCaution(false);
    inst->m_dsky->ai->setGimbalLockStatus(false);
    inst->m_dsky->ai->setProgramCondition(false);
    inst->m_dsky->ai->setRestartCondition(false);
    inst->m_dsky->ai->setTrackerCondition(false);
    inst->m_dsky->ai->setAltitudeDataCaution(false);
    inst->m_dsky->ai->setVelocityDataCaution(false);
    inst->m_dsky->di->setProgramNumber(DIGITAL_INDICATOR_VALUE_NAN);
  } else if (deltaMillis > 4800) {
    inst->m_dsky->ai->setKeyReleaseStatus(true);
    inst->m_dsky->ai->setOperatorErrorStatus(true);
    inst->m_dsky->di->setVerbCode("88");
    inst->m_dsky->di->setNounCode("88");
  } else if (deltaMillis > 4400) {
    inst->m_dsky->ai->setKeyReleaseStatus(false);
    inst->m_dsky->ai->setOperatorErrorStatus(false);
    inst->m_dsky->di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
    inst->m_dsky->di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
  } else if (deltaMillis > 3400) {
    inst->m_dsky->ai->setKeyReleaseStatus(true);
    inst->m_dsky->ai->setOperatorErrorStatus(true);
    inst->m_dsky->di->setVerbCode("88");
    inst->m_dsky->di->setNounCode("88");
  } else if (deltaMillis > 3000) {
    inst->m_dsky->ai->setKeyReleaseStatus(false);
    inst->m_dsky->ai->setOperatorErrorStatus(false);
    inst->m_dsky->di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
    inst->m_dsky->di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
  } else if (deltaMillis > 2000) {
    inst->m_dsky->ai->setKeyReleaseStatus(true);
    inst->m_dsky->ai->setOperatorErrorStatus(true);
    inst->m_dsky->di->setVerbCode("88");
    inst->m_dsky->di->setNounCode("88");
  } else if (deltaMillis > 1600) {
    inst->m_dsky->ai->setKeyReleaseStatus(false);
    inst->m_dsky->ai->setOperatorErrorStatus(false);
    inst->m_dsky->di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
    inst->m_dsky->di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
  } else if (deltaMillis > 600) {
    inst->m_dsky->ai->setKeyReleaseStatus(true);
    inst->m_dsky->ai->setOperatorErrorStatus(true);
    inst->m_dsky->di->setVerbCode("88");
    inst->m_dsky->di->setNounCode("88");
  } else if (deltaMillis > 500) {
    inst->m_dsky->di->setRegister3("+88888");
    inst->m_dsky->di->setProgramNumber("88");
  } else if (deltaMillis > 400) {
    inst->m_dsky->di->setRegister2("+88888");
  } else if (deltaMillis > 300) {
    inst->m_dsky->ai->setKeyReleaseStatus(false);
    inst->m_dsky->ai->setOperatorErrorStatus(false);
    inst->m_dsky->di->setVerbCode(DIGITAL_INDICATOR_VALUE_NAN);
    inst->m_dsky->di->setNounCode(DIGITAL_INDICATOR_VALUE_NAN);
    inst->m_dsky->di->setRegister1("+88888");
  } else if (deltaMillis > 200) {
    inst->m_dsky->di->setNounCode("88");
  } else if (deltaMillis > 100) {
    inst->m_dsky->ai->setUplinkActivityStatus(true);
    inst->m_dsky->ai->setNoAttitudeStatus(true);
    inst->m_dsky->ai->setStandbyStatus(true);
    inst->m_dsky->ai->setKeyReleaseStatus(true);
    inst->m_dsky->ai->setOperatorErrorStatus(true);
    inst->m_dsky->ai->setTemperatureCaution(true);
    inst->m_dsky->ai->setGimbalLockStatus(true);
    inst->m_dsky->ai->setProgramCondition(true);
    inst->m_dsky->ai->setRestartCondition(true);
    inst->m_dsky->ai->setTrackerCondition(true);
    inst->m_dsky->ai->setAltitudeDataCaution(true);
    inst->m_dsky->ai->setVelocityDataCaution(true);
    inst->m_dsky->di->setVerbCode("88");
  }
  return DSKY_STATE_BUSY;
}
