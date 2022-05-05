#ifndef DIGITALINDICATOR_H
#define DIGITALINDICATOR_H

#include <TFT_eSPI.h>

#define DIGITAL_INDICATOR_CS 26

#define DIGITAL_INDICATOR_VALUE_NAN "NAN"

class DigitalIndicator
{
public:
  DigitalIndicator(TFT_eSPI *tft, TFT_eSprite *spr);
  virtual ~DigitalIndicator();

  void resetIndicator(bool statup);
  void powerDownIndicator(void);

  void update(void);

  void setComputerActivityStatus(bool status);
  bool getComputerActivityStatus(void);

  void setProgramNumber(String number);
  String getProgramNumber(void);

  void setVerbCode(String code);
  String getVerbCode(void);
  void setVerbCodeBlinking(bool blinking);

  void setNounCode(String code);
  String getNounCode(void);
  void setNounCodeBlinking(bool blinking);

  void setRegister1(String value);
  String getRegister1(void);

  void setRegister2(String value);
  String getRegister2(void);

  void setRegister3(String value);
  String getRegister3(void);

private:
  TFT_eSPI *m_tft;
  TFT_eSprite *m_spr;

  bool m_verbCodeBlinking;
  bool m_nounCodeBlinking;

  bool m_compActyStatus;
  String m_programNumber;
  String m_verbCode;
  String m_nounCode;
  String m_registerValue[3];

  void printUInt8Value(uint16_t x, uint16_t y, String value);
  void printInt32Value(uint16_t x, uint16_t y, String value);

  void drawLayout(void);

  uint16_t m_toggleCounter;

};

#endif /* DIGITALINDICATOR_H */
