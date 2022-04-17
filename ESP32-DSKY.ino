#include <SPI.h>
#include <TFT_eSPI.h>

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"

AlarmIndicator *alarmInd;
DigitalIndicator *digitalInd;

TFT_eSPI tft = TFT_eSPI(320, 480);

bool acty = false;
int8_t register1 = -10;

void setup() {
  Serial.begin(115200);


  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(0);

  alarmInd = new AlarmIndicator(tft);
  digitalInd = new DigitalIndicator(tft);  
}

void loop() {
  digitalInd->setComputerActivityStatus(acty);
  digitalInd->setProgramNumber(63);
  digitalInd->setVerbCode(5);
  digitalInd->setNounCode(9);
  digitalInd->setRegister1(register1);
  digitalInd->setRegister2(88888);
  digitalInd->setRegister3(88888);
  acty = !acty;
  register1++;
  delay(1000);
}
