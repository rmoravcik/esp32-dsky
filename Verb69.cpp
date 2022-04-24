#include "ESP32-DSKY.h"
#include "Verb69.h"

uint8_t verb69_start(AlarmIndicator *ai, DigitalIndicator *di)
{
  Serial.print("VERB69 started at ");
  Serial.println(millis());
  return FAGC_BUSY;
}

uint8_t verb69_cycle(void)
{
  ESP.restart();
  return FAGC_IDLE;
}
