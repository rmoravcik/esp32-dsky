#include "ESP32-DSKY.h"
#include "Verb69.h"

uint8_t verb69_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  Serial.print("VERB69 started at ");
  Serial.println(millis());
  return FAGC_BUSY;
}

uint8_t verb69_cycle(char key, bool stop)
{
  ESP.restart();
  return FAGC_IDLE;
}
