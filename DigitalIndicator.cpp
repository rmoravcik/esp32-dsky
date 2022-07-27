#include "ESP32-DSKY.h"
#include "DigitalIndicator.h"

#include "Gorton-Normal-18011.h"
#include "Zerlina26pt7b.h"

#define TOGGLE_DELAY_MS (1000 / MAIN_LOOP_DELAY_MS)

#define TFT_GREEN_INDICATOR 0x068F

DigitalIndicator::DigitalIndicator(TFT_eSPI *tft, TFT_eSprite *spr)
{
  m_tft = tft;
  m_spr = spr;

  m_toggleCounter = 0;
  m_verbCodeBlinking = false;
  m_nounCodeBlinking = false;
  m_compActyStatus = true;
  m_programNumber = DIGITAL_INDICATOR_VALUE_NAN;
  m_verbCode = DIGITAL_INDICATOR_VALUE_NAN;
  m_nounCode = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[0] = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[1] = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[2] = DIGITAL_INDICATOR_VALUE_NAN;

  m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);

  drawLayout();
  drawNounLabel();
  setNounCode("88");
  delay(200);
  drawProgramLabel();
  setProgramNumber("88");
  delay(200);
  drawVerbLabel();
  setVerbCode("88");
  delay(200);
  setComputerActivityStatus(true);
}

DigitalIndicator::~DigitalIndicator()
{
}

void DigitalIndicator::drawLayout(void)
{
  m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);

  m_tft->fillScreen(TFT_BLACK);
  m_tft->loadFont(Gorton_Normal_180_11);

  // Dots
  m_tft->fillCircle(120, 15, 2, TFT_WHITE);
  m_tft->fillCircle(120, 75, 2, TFT_WHITE);
  m_tft->fillCircle(120, 136, 2, TFT_WHITE);
  m_tft->fillCircle(30, 157, 2, TFT_WHITE);
  m_tft->fillCircle(30, 213, 2, TFT_WHITE);
  m_tft->fillCircle(30, 269, 2, TFT_WHITE);
  m_tft->fillCircle(210, 157, 2, TFT_WHITE);
  m_tft->fillCircle(210, 213, 2, TFT_WHITE);
  m_tft->fillCircle(210, 269, 2, TFT_WHITE);
 
  // Separators
  m_tft->fillRoundRect(42, 153, 156, 4, 0, TFT_GREEN);
  m_tft->fillRoundRect(42, 209, 156, 4, 0, TFT_GREEN);
  m_tft->fillRoundRect(42, 265, 156, 4, 0, TFT_GREEN);
}

void DigitalIndicator::resetIndicator(void)
{
  m_toggleCounter = 0;
  m_verbCodeBlinking = false;
  m_nounCodeBlinking = false;
  m_compActyStatus = true;
  m_programNumber = DIGITAL_INDICATOR_VALUE_NAN;
  m_verbCode = DIGITAL_INDICATOR_VALUE_NAN;
  m_nounCode = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[0] = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[1] = DIGITAL_INDICATOR_VALUE_NAN;
  m_registerValue[2] = DIGITAL_INDICATOR_VALUE_NAN;

  drawLayout();

  m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);

  setComputerActivityStatus(false);
  drawProgramLabel();
  drawVerbLabel();
  drawNounLabel();
}
    
void DigitalIndicator::drawProgramLabel(void)
{
  m_tft->fillRoundRect(145, 5, 70, 20, 3, TFT_GREEN);
  m_tft->setCursor(161, 10);
  m_tft->loadFont(Gorton_Normal_180_11);
  m_tft->setTextColor(TFT_BLACK, TFT_GREEN);
  m_tft->print("PROG");
}

void DigitalIndicator::drawVerbLabel(void)
{
  m_tft->fillRoundRect(25, 81, 70, 20, 3, TFT_GREEN);
  m_tft->setCursor(40, 86);
  m_tft->loadFont(Gorton_Normal_180_11);
  m_tft->setTextColor(TFT_BLACK, TFT_GREEN);
  m_tft->print("VERB");
  m_tft->unloadFont();
}

void DigitalIndicator::drawNounLabel(void)
{
  m_tft->fillRoundRect(145, 81, 70, 20, 3, TFT_GREEN);
  m_tft->setCursor(160, 86);
  m_tft->loadFont(Gorton_Normal_180_11);
  m_tft->setTextColor(TFT_BLACK, TFT_GREEN);
  m_tft->print("NOUN");
}

void DigitalIndicator::powerDownIndicator(void)
{
  setVerbCodeBlinking(false);
  setNounCodeBlinking(false);
  drawLayout();
}

void DigitalIndicator::update(void)
{
  static bool toggle = true;

  if (!toggle) {
    if (m_toggleCounter > VERB_NOUN_OFF_DELAY_MS) {
      m_toggleCounter = 0;
      toggle = true;

      if (m_verbCodeBlinking) {
        printUInt8Value(25, 146, m_verbCode);
      }
      if (m_nounCodeBlinking) {
        printInt32Value(145, 146, m_nounCode);
      }
    }
  } else {
    if (m_toggleCounter > VERB_NOUN_ON_DELAY_MS) {
      m_toggleCounter = 0;
      toggle = false;

      if (m_verbCodeBlinking) {
        printUInt8Value(25, 146, DIGITAL_INDICATOR_VALUE_NAN);
      }
      if (m_nounCodeBlinking) {
        printInt32Value(145, 146, DIGITAL_INDICATOR_VALUE_NAN);
      }
    }
  }

  m_toggleCounter++;
}

void DigitalIndicator::setComputerActivityStatus(bool status)
{
  uint32_t buttonColor = TFT_BLACK;
  uint32_t textColor = TFT_DARKGREY;

  if (m_compActyStatus != status) {
    m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);

    if (status == true) {
      buttonColor = TFT_GREEN;
      textColor = TFT_BLACK;
    }

    m_tft->fillRoundRect(25, 5, 70, 65, 3, buttonColor);
    m_tft->loadFont(Gorton_Normal_180_11);
    m_tft->setTextColor(textColor, buttonColor);
    m_tft->setCursor(41, 25);
    m_tft->print("COMP");
    m_tft->setCursor(41, 40);
    m_tft->print("ACTY");
    m_tft->unloadFont();

    m_compActyStatus = status;
  }
}

bool DigitalIndicator::getComputerActivityStatus(void)
{
  return m_compActyStatus;
}

void DigitalIndicator::setProgramNumber(String number)
{
  if (m_programNumber != number) {
     printUInt8Value(145, 70, number);
    m_programNumber = number;
  }
}

String DigitalIndicator::getProgramNumber(void)
{
  return m_programNumber;
}

void DigitalIndicator::setVerbCode(String code)
{
  if (m_verbCode != code) {
    printUInt8Value(25, 146, code);
    m_verbCode = code;
  }
}

String DigitalIndicator::getVerbCode(void)
{
  return m_verbCode;
}

void DigitalIndicator::setVerbCodeBlinking(bool blinking)
{
  m_verbCodeBlinking = blinking;
  if (!m_verbCodeBlinking) {
    printUInt8Value(25, 146, m_verbCode);
  }
}

void DigitalIndicator::setNounCode(String code)
{
  if (m_nounCode != code) {
    printUInt8Value(145, 146, code);
    m_nounCode = code;
  }
}

String DigitalIndicator::getNounCode(void)
{
  return m_nounCode;
}

void DigitalIndicator::setNounCodeBlinking(bool blinking)
{
  m_nounCodeBlinking = blinking;
  if (!m_nounCodeBlinking) {
    printUInt8Value(145, 146, m_nounCode);
  }
}

void DigitalIndicator::setRegister1(String value)
{
  if (m_registerValue[0] != value) {
    printInt32Value(25, 203, value);
    m_registerValue[0] = value;
  }
}

String DigitalIndicator::getRegister1(void)
{
  return m_registerValue[0];
}

void DigitalIndicator::setRegister2(String value)
{
  if (m_registerValue[1] != value) {
    printInt32Value(25, 259, value);
    m_registerValue[1] = value;  
  }
}

String DigitalIndicator::getRegister2(void)
{
  return m_registerValue[1];
}

void DigitalIndicator::setRegister3(String value)
{
  if (m_registerValue[2] != value) {
    printInt32Value(25, 315, value);
    m_registerValue[2] = value;
  }
}

String DigitalIndicator::getRegister3(void)
{
  return m_registerValue[2];
}

void DigitalIndicator::printUInt8Value(uint16_t x, uint16_t y, String value)
{  
  char str[3];
  int ret;

  m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);

  m_spr->setFreeFont(&Zerlina26pt7b);
  m_spr->createSprite(69, 44);

  m_spr->fillSprite(TFT_BLACK);

  if (value != DIGITAL_INDICATOR_VALUE_NAN) {
    m_spr->setTextColor(TFT_GREEN, TFT_BLACK);
    ret = snprintf(str, sizeof(str), "%s", value.c_str());

    if (ret < 0) {
      Serial.print("DigitalIndicator::printInt32Value(): ERROR=");
      Serial.println(ret);
    }

    m_spr->drawString(str, 0, 0);
  }

  m_spr->pushSprite(x, y - 44 + 3);
  m_spr->deleteSprite();
}
void DigitalIndicator::printInt32Value(uint16_t x, uint16_t y, String value)
{
  char str[7];
  int ret;

  m_tft->TFT_CS_MASK = (1 << GPIO_DIGITAL_INDICATOR_CS);
  
  m_spr->setFreeFont(&Zerlina26pt7b);
  m_spr->createSprite(184, 44);

  m_spr->fillSprite(TFT_BLACK);

  if (value != DIGITAL_INDICATOR_VALUE_NAN) {
    m_spr->setTextColor(TFT_GREEN, TFT_BLACK);
    ret = snprintf(str, sizeof(str), "%s", value.c_str());

    if (ret < 0) {
      Serial.print("DigitalIndicator::printInt32Value(): ERROR=");
      Serial.println(ret);
    }

    m_spr->drawString(str, 0, 0);
  }

  m_spr->pushSprite(x, y - 44 + 3);
  m_spr->deleteSprite();
}
