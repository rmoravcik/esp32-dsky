#include "ESP32-DSKY.h"
#include "AlarmIndicator.h"

#include "Gorton-Normal-12014.h"

#define TOGGLE_DELAY_MS (1000 / MAIN_LOOP_DELAY_MS)
// For some reason R and B channels are inverted
#define TFT_ALARM_YELLOW TFT_CYAN

AlarmIndicator::AlarmIndicator(TFT_eSPI *tft)
{
  m_tft = tft;

  resetIndicator();
}

AlarmIndicator::~AlarmIndicator()
{
}

void AlarmIndicator::resetIndicator(void)
{
#ifdef ESP32
  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  m_tft->setRotation(1);
  m_tft->fillScreen(TFT_BLACK);

  m_toggleCounter = 0;

  m_uplinkActivityStatus = true;
  m_noAttitudeStatus = true;
  m_standbyStatus = true;
  m_keyReleaseStatus = true;
  m_operatorErrorStatus = true;

  m_keyReleaseStatusBlinking = false;
  m_operatorErrorStatusBlinking = false;

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
#endif /* ESP32 */  
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
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_uplinkActivityStatus == status) {
    return;
  }
  m_uplinkActivityStatus = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

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
#endif /* ESP32 */
}

void AlarmIndicator::setNoAttitudeStatus(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_noAttitudeStatus == status) {
    return;
  }
  m_noAttitudeStatus = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

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
#endif /* ESP32 */
}

void AlarmIndicator::setStandbyStatus(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_standbyStatus == status) {
    return;
  }
  m_standbyStatus = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

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
#endif /* ESP32 */
}

void AlarmIndicator::_setKeyReleaseStatus(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

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
#endif /* ESP32 */
}

void AlarmIndicator::_setOperatorErrorStatus(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

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
#endif /* ESP32 */
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
  m_keyReleaseStatusBlinking = true;
}

void AlarmIndicator::setOperatorErrorStatusBlinking(void)
{
  m_operatorErrorStatusBlinking = true;
}

void AlarmIndicator::setTemperatureCaution(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_temperatureCaution == status) {
    return;
  }
  m_temperatureCaution = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 7, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(150, 18);
  m_tft->print("TEMP");
  m_tft->unloadFont();
#endif /* ESP32 */
}

void AlarmIndicator::setGimbalLockStatus(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_gimbalLockStatus == status) {
    return;
  }
  m_gimbalLockStatus = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
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
#endif /* ESP32 */
}

void AlarmIndicator::setProgramCondition(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_programCondition == status) {
    return;
  }
  m_programCondition = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 97, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(152, 108);
  m_tft->print("PROG");
  m_tft->unloadFont();
#endif /* ESP32 */
}

void AlarmIndicator::setRestartCondition(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_restartCondition == status) {
    return;
  }
  m_restartCondition = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 142, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(138, 153);
  m_tft->print("RESTART");
  m_tft->unloadFont();
#endif /* ESP32 */
}

void AlarmIndicator::setTrackerCondition(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_trackerCondition == status) {
    return;
  }
  m_trackerCondition = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 187, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(136, 198);
  m_tft->print("TRACKER");
  m_tft->unloadFont();
#endif /* ESP32 */
}

void AlarmIndicator::setAltitudeDataCaution(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_altitudeDataCaution == status) {
    return;
  }
  m_altitudeDataCaution = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 232, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(157, 243);
  m_tft->print("ALT");
  m_tft->unloadFont();
#endif /* ESP32 */
}

void AlarmIndicator::setVelocityDataCaution(bool status)
{
#ifdef ESP32
  uint32_t buttonColor = TFT_LIGHTGREY;
  uint32_t textColor = TFT_DARKGREY;

  if (m_velocityDataCaution == status) {
    return;
  }
  m_velocityDataCaution = status;

  m_tft->TFT_CS_MASK = (1 << ALARM_INDICATOR_CS);

  if (status == true) {
    buttonColor = TFT_ALARM_YELLOW;
    textColor = TFT_BLACK;
  }

  m_tft->fillRoundRect(125, 277, 90, 35, 3, buttonColor);
  m_tft->loadFont(Gorton_Normal_120_14);
  m_tft->setTextColor(textColor, buttonColor);
  m_tft->setCursor(157, 288);
  m_tft->print("VEL");
  m_tft->unloadFont();
#endif /* ESP32 */
}
