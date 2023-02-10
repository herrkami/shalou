#ifndef _JIKAN_SLEEP_H_
#define _JIKAN_SLEEP_H_

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "jikan_io.h"
#include "jikan_jobs.h"
#include "SSD1306.h"

void configureSleepMode(void);
void activateSleepMode(void);

#endif
