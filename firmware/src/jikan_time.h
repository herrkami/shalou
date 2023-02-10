#ifndef _JIKAN_TIME_H_
#define _JIKAN_TIME_H_

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "jikan_io.h"

const uint32_t ONE_SECOND = 500;
const uint32_t ONE_MINUTE = 60*ONE_SECOND;

extern volatile uint32_t _wallclock;
extern volatile uint32_t wallclock;

typedef struct {
    uint8_t minutes;
    uint8_t seconds;
} Time;

void initWallClock(void);

#endif
