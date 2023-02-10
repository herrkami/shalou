#ifndef _JIKAN_JOBS_H_
#define _JIKAN_JOBS_H_

#include "jikan_io.h"
#include "jikan_ui.h"
#include "jikan_time.h"
#include "jikan_sleep.h"
#include "jikan_display.h"

#define NR_JOBS 7
#define JOB_IDX_FADE_MAIN 0
#define JOB_IDX_FADE_BREATH 1
#define JOB_IDX_SHUTDOWN_TIMER 2
#define JOB_IDX_BUTTONS 3
#define JOB_IDX_SET_TIMER 4
#define JOB_IDX_COUNTDOWN 5
#define JOB_IDX_ALARM 6

#define JOB_INACTIVE 0
#define JOB_ACTIVE 1

#define SLOPE_RISING 0
#define SLOPE_FALLING 1

#define NR_TIMER_PERIODS 25
const uint32_t timer_period[] = {
    0*ONE_SECOND,
    10*ONE_SECOND,
    20*ONE_SECOND,
    30*ONE_SECOND,
    40*ONE_SECOND,
    50*ONE_SECOND,
    60*ONE_SECOND,
    90*ONE_SECOND,
    2*ONE_MINUTE,
    3*ONE_MINUTE,
    4*ONE_MINUTE,
    5*ONE_MINUTE,
    6*ONE_MINUTE,
    7*ONE_MINUTE,
    8*ONE_MINUTE,
    9*ONE_MINUTE,
    10*ONE_MINUTE,
    12*ONE_MINUTE,
    15*ONE_MINUTE,
    20*ONE_MINUTE,
    25*ONE_MINUTE,
    30*ONE_MINUTE,
    45*ONE_MINUTE,
    60*ONE_MINUTE,
    90*ONE_MINUTE,
};

typedef struct {
    uint8_t active;
    uint32_t delta_t;
    uint32_t t_next;
    uint32_t runtime;
    void (*event_handler)();
} JobState;

typedef struct {
    uint32_t period;
    uint32_t start;
    uint8_t slope;
    uint8_t brightness_divider;
} FadeState;

typedef struct {
    uint8_t period_idx;
    uint32_t period;
    uint32_t start;
    Display *display;
} CountDownState;

extern JobState job_state[NR_JOBS];

extern FadeState fade_state_main;
extern FadeState fade_state_breath;

extern CountDownState count_down_state;

void initJobs(void);
void handleJobs(void);

void initCountDownState(void);

void initFadeStates(void);
void handleFadeMain(void);
void handleFadeBreath(void);

void handleShutdownTimer(void);
void resetShutdownTimer(void);

void enterSetTimerMode(void);
void handleSetTimer(void);
void incrementTimer(void);
void decrementTimer(void);
void enterCountDownMode(void);
void pauseCountDown(void);
void handleCountDown(void);
void enterAlarmMode(void);
void handleAlarm(void);
#endif
