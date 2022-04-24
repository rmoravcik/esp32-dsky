#ifndef DIGITALINDICATOR_H
#define DIGITALINDICATOR_H

#include <TFT_eSPI.h>

#define DIGITAL_INDICATOR_CS PIN_D2

#define DIGITAL_INDICATOR_VALUE_UINT8_NAN     0xFFU
#define DIGITAL_INDICATOR_REGISTER_VALUE_NAN  INT32_MIN

class DigitalIndicator
{
public:
  DigitalIndicator(TFT_eSPI *tft, TFT_eSprite *spr);
  virtual ~DigitalIndicator();

  void resetIndicator(void);

  void setComputerActivityStatus(bool status);

  void setProgramNumber(uint8_t number);
  uint8_t getProgramNumber(void);

  void setVerbCode(uint8_t code);
  uint8_t getVerbCode(void);

  void setNounCode(uint8_t code);
  uint8_t getNounCode(void);

  void setRegister1(int32_t value);
  int32_t getRegister1(void);

  void setRegister2(int32_t value);
  int32_t getRegister2(void);

  void setRegister3(int32_t value);
  int32_t getRegister3(void);

private:
  TFT_eSPI *m_tft;
  TFT_eSprite *m_spr;

  bool m_compActyStatus;
  uint8_t m_programNumber;
  uint8_t m_verbCode;
  uint8_t m_nounCode;
  int32_t m_registerValue[3];

  void printUInt8Value(uint16_t x, uint16_t y, uint8_t value);
  void printInt32Value(uint16_t x, uint16_t y, int32_t value);

};

#endif /* DIGITALINDICATOR_H */
