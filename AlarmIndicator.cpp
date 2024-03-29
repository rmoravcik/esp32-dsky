#include "ESP32-DSKY.h"
#include "AlarmIndicator.h"

#include "Gorton-Normal-12014.h"

#define TOGGLE_DELAY_MS (1000 / MAIN_LOOP_DELAY_MS)

// For some reason R and B channels are inverted
#define TFT_ALARM_YELLOW_ACTIVE     TFT_CYAN
#define ALARM_TEXT_COLOR_INACTIVE   TFT_BLACK
#define ALARM_BUTTON_COLOR_INACTIVE 0x0841

AlarmIndicator::AlarmIndicator(TFT_eSPI *tft, bool *standbyActive)
{
  m_tft = tft;
  m_standbyActive = standbyActive;

  m_toggleCounter = 0;

  m_keyReleaseStatusBlinking = false;
  m_operatorErrorStatusBlinking = false;

  m_uplinkActivityStatus = false;
  m_noAttitudeStatus = false;
  m_standbyStatus = false;
  m_keyReleaseStatus = false;
  m_operatorErrorStatus = false;

  m_temperatureCaution = false;
  m_gimbalLockStatus = false;
  m_programCondition = false;
  m_restartCondition = false;
  m_trackerCondition = false;
  m_altitudeDataCaution = false;
  m_velocityDataCaution = false;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);
  m_tft->fillScreen(TFT_BLACK);

  setTemperatureCaution(true);
  delay(50);
  setGimbalLockStatus(true);
  delay(50);
  setProgramCondition(true);
  delay(50);
  setRestartCondition(true);
  delay(50);
  setTrackerCondition(true);
  delay(50);
  setAltitudeDataCaution(true);
  delay(50);
  setVelocityDataCaution(true);
  delay(50);    
  setOperatorErrorStatus(true);
  delay(50);    
  setKeyReleaseStatus(true);
  delay(50);    
  setStandbyStatus(true);
  delay(50);    
  setNoAttitudeStatus(true);
  delay(50);    
  setUplinkActivityStatus(true);
}

AlarmIndicator::~AlarmIndicator()
{
}

void AlarmIndicator::resetIndicator(void)
{
  m_toggleCounter = 0;

  m_keyReleaseStatusBlinking = false;
  m_operatorErrorStatusBlinking = false;

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
}

void AlarmIndicator::powerDownIndicator(void)
{
  setUplinkActivityStatus(false);
  setNoAttitudeStatus(false);
  setStandbyStatus(true);
  setKeyReleaseStatus(false);
  setOperatorErrorStatus(false);

  setTemperatureCaution(false);
  setGimbalLockStatus(false);
  setProgramCondition(false);
  setRestartCondition(false);
  setTrackerCondition(false);
  setAltitudeDataCaution(false);
  setVelocityDataCaution(false);
}

void AlarmIndicator::update(void)
{
  static bool toggle = true;

  if (m_toggleCounter == TOGGLE_DELAY_MS) {
    if (toggle) {
      toggle = false;

      if (m_keyReleaseStatusBlinking) {
        _setKeyReleaseStatus(true);
      }
      if (m_operatorErrorStatusBlinking) {
        _setOperatorErrorStatus(true);
      }
    } else {
      toggle = true;

      if (m_keyReleaseStatusBlinking) {
        _setKeyReleaseStatus(false);
      }
      if (m_operatorErrorStatusBlinking) {
        _setOperatorErrorStatus(false);
      }
    }

    m_toggleCounter = 0;
  }

  m_toggleCounter++;
}

void AlarmIndicator::setUplinkActivityStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if (m_uplinkActivityStatus == status) {
    return;
  }
  m_uplinkActivityStatus = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(25, 7, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(48, 11);
  m_tft->print("UPLINK");
  m_tft->setCursor(52, 26);
  m_tft->print("ACTY");    
  m_tft->unloadFont();
}

void AlarmIndicator::setNoAttitudeStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_noAttitudeStatus == status) || (*m_standbyActive == true)) {
    return;
  }
  m_noAttitudeStatus = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(25, 52, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(46, 63);
  m_tft->print("NO ATT");
  m_tft->unloadFont();
}

void AlarmIndicator::setStandbyStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_standbyStatus == status) || (*m_standbyActive == true)) {
    return;
  }
  m_standbyStatus = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(25, 97, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(52, 108);
  m_tft->print("STBY");
  m_tft->unloadFont();
}

void AlarmIndicator::_setKeyReleaseStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(25, 142, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(40, 153);
  m_tft->print("KEY   REL");
  m_tft->unloadFont();
}

void AlarmIndicator::_setOperatorErrorStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_WHITE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(25, 187, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(37, 198);
  m_tft->print("OPR   ERR");
  m_tft->unloadFont();
}

void AlarmIndicator::setKeyReleaseStatus(bool status)
{
  if ((m_keyReleaseStatusBlinking) && (status == false)) {
    m_keyReleaseStatusBlinking = false;
    _setKeyReleaseStatus(status);
  } else if (m_keyReleaseStatus != status) {
    m_keyReleaseStatus = status;
    _setKeyReleaseStatus(status);
  }
}

void AlarmIndicator::setOperatorErrorStatus(bool status)
{
  if ((m_operatorErrorStatusBlinking) && (status == false)) {
    m_operatorErrorStatusBlinking = false;
    _setOperatorErrorStatus(status);
  } else if (m_operatorErrorStatus != status) {
    m_operatorErrorStatus = status;
    _setOperatorErrorStatus(status);
  }
}

void AlarmIndicator::setKeyReleaseStatusBlinking(void)
{
  setKeyReleaseStatus(true);
  m_keyReleaseStatusBlinking = true;
}

void AlarmIndicator::setOperatorErrorStatusBlinking(void)
{
  setOperatorErrorStatus(true);
  m_operatorErrorStatusBlinking = true;
}

void AlarmIndicator::setTemperatureCaution(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if (m_temperatureCaution == status) {
    return;
  }
  m_temperatureCaution = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 7, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(150, 18);
  m_tft->print("TEMP");
  m_tft->unloadFont();
}

void AlarmIndicator::setGimbalLockStatus(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_gimbalLockStatus == status) || (*m_standbyActive == true)) {
    return;
  }
  m_gimbalLockStatus = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 52, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(145, 56);
  m_tft->print("GIMBAL");
  m_tft->setCursor(152, 71);
  m_tft->print("LOCK");
  m_tft->unloadFont();
}

void AlarmIndicator::setProgramCondition(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_programCondition == status) || (*m_standbyActive == true)) {
    return;
  }
  m_programCondition = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 97, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(152, 108);
  m_tft->print("PROG");
  m_tft->unloadFont();
}

void AlarmIndicator::setRestartCondition(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_restartCondition == status) || (*m_standbyActive == true)) {
    return;
  }
  m_restartCondition = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 142, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(138, 153);
  m_tft->print("RESTART");
  m_tft->unloadFont();
}

void AlarmIndicator::setTrackerCondition(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_trackerCondition == status) || (*m_standbyActive == true)) {
    return;
  }
  m_trackerCondition = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 187, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(136, 198);
  m_tft->print("TRACKER");
  m_tft->unloadFont();
}

void AlarmIndicator::setAltitudeDataCaution(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_altitudeDataCaution == status) || (*m_standbyActive == true)) {
    return;
  }
  m_altitudeDataCaution = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 232, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(157, 243);
  m_tft->print("ALT");
  m_tft->unloadFont();
}

void AlarmIndicator::setVelocityDataCaution(bool status)
{
  uint32_t buttonColor = ALARM_BUTTON_COLOR_INACTIVE;
  uint32_t textColor = ALARM_TEXT_COLOR_INACTIVE;

  if ((m_velocityDataCaution == status) || (*m_standbyActive == true)) {
    return;
  }
  m_velocityDataCaution = status;

  m_tft->TFT_CS_MASK = (1 << GPIO_ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW_ACTIVE;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 277, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(157, 288);
  m_tft->print("VEL");
  m_tft->unloadFont();
}
