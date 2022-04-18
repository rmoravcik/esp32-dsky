#include "AlarmIndicator.h"

#include "Gorton-Normal-12014.h"

#define AI_TFT_CS PIN_D8

AlarmIndicator::AlarmIndicator(TFT_eSPI &tft) : m_tft(tft)
{
  pinMode(AI_TFT_CS, OUTPUT);
  digitalWrite(AI_TFT_CS, LOW);

  // REMOVE ME
  m_tft.drawRect(0, 0, 240, 320, TFT_WHITE);

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(48, 11);
  m_tft.print("UPLINK");
  m_tft.setCursor(52, 26);
  m_tft.print("ACTY");    
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(46, 63);
  m_tft.print("NO ATT");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(52, 108);
  m_tft.print("STBY");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(40, 153);
  m_tft.print("KEY   REL");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(37, 198);
  m_tft.print("OPR   ERR");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(150, 18);
  m_tft.print("TEMP");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(145, 56);
  m_tft.print("GIMBAL");
  m_tft.setCursor(152, 71);
  m_tft.print("LOCK");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(152, 108);
  m_tft.print("PROG");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(138, 153);
  m_tft.print("RESTART");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(136, 198);
  m_tft.print("TRACKER");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(157, 243);
  m_tft.print("ALT");
  m_tft.unloadFont();

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
  m_tft.loadFont(Gorton_Normal_120_14);
  m_tft.setTextColor(textColor, buttonColor);
  m_tft.setCursor(157, 288);
  m_tft.print("VEL");
  m_tft.unloadFont();

  digitalWrite(AI_TFT_CS, HIGH);
}
