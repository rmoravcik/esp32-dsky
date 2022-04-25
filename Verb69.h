#ifndef VERB69_H
#define VERB69_H

#include "AlarmIndicator.h"
#include "DigitalIndicator.h"
#include "Weather.h"

uint8_t verb69_start(AlarmIndicator *ai, DigitalIndicator *di, Weather *weather);
uint8_t verb69_cycle(void);

#endif /* VERB69 */
