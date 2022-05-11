#include "Verb36.h"

static Verb36 *inst = NULL;

Verb36::Verb36(DSKY *dsky)
{
  m_dsky = dsky;
}

Verb36::~Verb36()
{
}

uint8_t verb36_start(DSKY *dsky)
{
  if (inst == NULL) {
    inst = new Verb36(dsky);
  }

  Serial.print("VERB36 started at ");
  Serial.println(millis());

  return DSKY_STATE_BUSY;
}

uint8_t verb36_cycle(char key, bool stopRequested, uint8_t state)
{
  inst->m_dsky->ai->resetIndicator();
  inst->m_dsky->di->resetIndicator();
  return DSKY_STATE_INIT;
}
