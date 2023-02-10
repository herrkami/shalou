#ifndef _JIKAN_UI_H_
#define _JIKAN_UI_H_

// #include <avr/sleep.h>
// #include <avr/interrupt.h>

#include "jikan_io.h"
#include "jikan_sleep.h"
#include "jikan_jobs.h"

#define NR_BUTTONS 3
#define BUTTON_MAIN_IDX 0
#define BUTTON_LEFT_IDX 1
#define BUTTON_RIGHT_IDX 2

#define HOLD_CTR_MAX   1000
#define HOLD_CTR_DETECT 100

// State data types and structures
typedef struct {
    uint8_t state_current;
    uint8_t state_previous;
    uint8_t pressed;
    uint8_t hold_event_happend;
    uint16_t hold_ctr;
    uint8_t (*button_handler)();
    void (*event_handler)();
    void (*hold_event_handler)();
} ButtonState;

extern ButtonState button_state[NR_BUTTONS];

void assignButtonHandlers(void);
void assignButtonEvents(void);
void assignButtonHoldEvents(void);
void updateButtonStates(void);
void initUI(void);
void doNothing(void);

#endif
