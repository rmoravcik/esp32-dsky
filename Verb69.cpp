#include "ESP32-DSKY.h"
#include "Verb69.h"

uint8_t verb69_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  Serial.print("VERB69 started at ");
  Serial.println(millis());
  return DSKY_STATE_BUSY;
}

uint8_t verb69_cycle(char key, bool stopRequested, uint8_t state)
{
  ledcWrite(0, 0);
  ESP.restart();
  return DSKY_STATE_IDLE;
}
