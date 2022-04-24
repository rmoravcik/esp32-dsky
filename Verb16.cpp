#include "ESP32-DSKY.h"
#include "Verb16.h"

static Verb16 *inst = NULL;

Verb16::Verb16(AlarmIndicator *ai, DigitalIndicator *di)
{
  m_ai = ai;
  m_di = di;

}

Verb16::~Verb16()
{
}

uint8_t verb16noun36_start(AlarmIndicator *ai, DigitalIndicator *di)
{
  if (inst == NULL) {
    inst = new Verb16(ai, di);
  }

  inst->startMillis = millis();

  Serial.print("VERB16 started at ");
  Serial.println(inst->startMillis);

  return FAGC_BUSY;
}

uint8_t verb16noun36_cycle(void)
{
  return FAGC_IDLE;
}
