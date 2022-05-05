#include "ESP32-DSKY.h"
#include "Program00.h"
#include "Program06.h"
#include "Verb37.h"

static Verb37 *inst = NULL;

static startFn_t programStartFn = 0;
cycleFn_t programCycleFn = 0;

struct program programs[] = {
  {  0, program00_start, program00_cycle },
  {  6, program06_start, program06_cycle },
  { -1,               0,               0 }
};

void findProgramStartCycleFunctions(int8_t programNumber, startFn_t *startFn, cycleFn_t *cycleFn)
{
  if (programNumber != PROGRAM_NUMBER_INVALID) {
    for (uint8_t program = 0; programs[program].number != PROGRAM_NUMBER_INVALID; program++) {
      if (programs[program].number == programNumber) {
          *startFn = programs[program].startFn;
          *cycleFn = programs[program].cycleFn;
          break;
      }
    }

    if ((programNumber != 0) && (*startFn == 0))
    {
      Serial.print("PROGRAM: program not found:");
      Serial.println(programNumber);
      inst->m_ai->setOperatorErrorStatusBlinking();
    }
  }
}

Verb37::Verb37(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  m_ai = ai;
  m_di = di;
}

Verb37::~Verb37()
{
}

uint8_t verb37noun00_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb37(ai, di, weather);
  }

  Serial.print("VERB37NOUN00 started at ");
  Serial.println(millis());

  findProgramStartCycleFunctions(0, &programStartFn, &programCycleFn);

  if (programStartFn) {
    di->setProgramNumber("00");
    programStartFn(ai, di, weather);
    programStartFn = 0;
  }

  return FAGC_BUSY;
}

uint8_t verb37noun00_cycle(char key, bool stop)
{
  return FAGC_IDLE;
}

uint8_t verb37noun06_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather)
{
  if (inst == NULL) {
    inst = new Verb37(ai, di, weather);
  }

  Serial.print("VERB37NOUN06 started at ");
  Serial.println(millis());

  findProgramStartCycleFunctions(6, &programStartFn, &programCycleFn);

  if (programStartFn) {
    di->setProgramNumber("06");
    programStartFn(ai, di, weather);
    programStartFn = 0;
  }

  return FAGC_BUSY;
}

uint8_t verb37noun06_cycle(char key, bool stop)
{
  return FAGC_IDLE;
}
