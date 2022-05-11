#ifndef VERB69_H
#define VERB69_H

#include "ESP32-DSKY.h"

uint8_t verb69_start(DSKY *dsky);
uint8_t verb69_cycle(char key, bool stopRequested, uint8_t state);

#endif /* VERB69 */
