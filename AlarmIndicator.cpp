#include "AlarmIndicator.h"

#include "Gorton_Normal_120_7pt7b.h"

#define AI_TFT_CS PIN_D8

AlarmIndicator::AlarmIndicator(TFT_eSPI &tft) : m_tft(tft)
{
  pinMode(AI_TFT_CS, OUTPUT);
  digitalWrite(AI_TFT_CS, LOW);

  // REMOVE ME
  m_tft.drawRect(0, 0, 240, 320, TFT_WHITE);

  m_tft.setFreeFont(&Gorton_Normal_1207pt7b);

  m_uplinkActivityStatus = true;
  m_noAttitudeStatus = true;
  m_standbyStatus = true;
  m_keyReleaseStatus = true;
  m_operatorErrorStatus = true;

  m_temperatureCaution = true;
  m_gimbalLockStatus = true;
  m_programCondition = true;
  m_restartCondition = true;
  m_trackerCondition = true;
  m_altitudeDataCaution = true;
  m_velocityDataCaution = true;

  setUplinkActivityStatus(false);
  setNoAttitudeStatus(false);
  setStandbyStatus(false);
  setKeyReleaseStatus(false);
  setOperatorErrorStatus(false);

  setTemperatureCaution(false);
  setGimbalLockStatus(false);
  setProgramCondition(false);
  setRestartCondition(false);
  setTrackerCondition(false);
  setAltitudeDataCaution(false);
  setVelocityDataCaution(false);

  digitalWrite(AI_TFT_CS, HIGH);
}

AlarmIndicator::~AlarmIndicator()
{
}

void AlarmIndicator::setUplinkActivityStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_uplinkActivityStatus == status) {
    return;
  }
  m_uplinkActivityStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(25, 7, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(47, 21);
  m_tft.print("UPLINK");
  m_tft.setCursor(52, 36);
  m_tft.print("ACTY");    

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setNoAttitudeStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_noAttitudeStatus == status) {
    return;
  }
  m_noAttitudeStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(25, 52, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(46, 74);
  m_tft.print("NO ATT");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setStandbyStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_standbyStatus == status) {
    return;
  }
  m_standbyStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(25, 97, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(52, 119);
  m_tft.print("STBY");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setKeyReleaseStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_keyReleaseStatus == status) {
    return;
  }
  m_keyReleaseStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(25, 142, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(40, 164);
  m_tft.print("KEY   REL");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setOperatorErrorStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_operatorErrorStatus == status) {
    return;
  }
  m_operatorErrorStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(25, 187, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(37, 209);
  m_tft.print("OPR   ERR");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setTemperatureCaution(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_temperatureCaution == status) {
    return;
  }
  m_temperatureCaution = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 7, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(150, 29);
  m_tft.print("TEMP");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setGimbalLockStatus(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_gimbalLockStatus == status) {
    return;
  }
  m_gimbalLockStatus = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 52, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(145, 67);
  m_tft.print("GIMBAL");
  m_tft.setCursor(152, 82);
  m_tft.print("LOCK");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setProgramCondition(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_programCondition == status) {
    return;
  }
  m_programCondition = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 97, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(152, 119);
  m_tft.print("PROG");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setRestartCondition(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_restartCondition == status) {
    return;
  }
  m_restartCondition = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 142, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(138, 163);
  m_tft.print("RESTART");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setTrackerCondition(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_trackerCondition == status) {
    return;
  }
  m_trackerCondition = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 187, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(138, 208);
  m_tft.print("TRACKER");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setAltitudeDataCaution(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_altitudeDataCaution == status) {
    return;
  }
  m_altitudeDataCaution = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 232, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(157, 253);
  m_tft.print("ALT");

  digitalWrite(AI_TFT_CS, HIGH);
}

void AlarmIndicator::setVelocityDataCaution(bool status)
{
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_velocityDataCaution == status) {
    return;
  }
  m_velocityDataCaution = status;

  digitalWrite(AI_TFT_CS, LOW);

  if (status == true) {
    buttonColor = TFT_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft.fillRoundRect(125, 277, 90, 35, 3, buttonColor);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(157, 298);
  m_tft.print("VEL");

  digitalWrite(AI_TFT_CS, HIGH);
}
