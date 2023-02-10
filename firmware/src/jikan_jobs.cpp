#include "jikan_jobs.h"

JobState job_state[NR_JOBS];
FadeState fade_state_main;
FadeState fade_state_breath;
CountDownState count_down_state;

// A job calls a function (event handler) with a fixed time interval.
// It can be active or inactive.

void initJobs(void){
    // LED main
    // job_state[JOB_IDX_FADE_MAIN].active = JOB_ACTIVE;
    job_state[JOB_IDX_FADE_MAIN].active = JOB_INACTIVE;
    job_state[JOB_IDX_FADE_MAIN].delta_t = 0.04*ONE_SECOND;
    job_state[JOB_IDX_FADE_MAIN].t_next = wallclock + job_state[JOB_IDX_FADE_MAIN].delta_t;
    job_state[JOB_IDX_FADE_MAIN].event_handler = handleFadeMain;

    // LED breath
    job_state[JOB_IDX_FADE_BREATH].active = JOB_ACTIVE;
    job_state[JOB_IDX_FADE_BREATH].delta_t = 0.04*ONE_SECOND;
    job_state[JOB_IDX_FADE_BREATH].t_next = wallclock + job_state[JOB_IDX_FADE_BREATH].delta_t;
    job_state[JOB_IDX_FADE_BREATH].event_handler = handleFadeBreath;

    // Shutdown timer
    job_state[JOB_IDX_SHUTDOWN_TIMER].active = JOB_ACTIVE;
    job_state[JOB_IDX_SHUTDOWN_TIMER].delta_t = 3*ONE_MINUTE;
    job_state[JOB_IDX_SHUTDOWN_TIMER].t_next = wallclock + job_state[JOB_IDX_SHUTDOWN_TIMER].delta_t;
    job_state[JOB_IDX_SHUTDOWN_TIMER].event_handler = handleShutdownTimer;

    // Button update
    job_state[JOB_IDX_BUTTONS].active = JOB_ACTIVE;
    job_state[JOB_IDX_BUTTONS].delta_t = 0.01*ONE_SECOND;
    job_state[JOB_IDX_BUTTONS].t_next = wallclock + job_state[JOB_IDX_BUTTONS].delta_t;
    job_state[JOB_IDX_BUTTONS].event_handler = updateButtonStates;

    // Set timer
    job_state[JOB_IDX_SET_TIMER].active = JOB_ACTIVE;
    job_state[JOB_IDX_SET_TIMER].delta_t = 0.04*ONE_SECOND;
    job_state[JOB_IDX_SET_TIMER].t_next = wallclock + job_state[JOB_IDX_SET_TIMER].delta_t;
    job_state[JOB_IDX_SET_TIMER].event_handler = handleSetTimer;

    // Countdown
    job_state[JOB_IDX_COUNTDOWN].active = JOB_INACTIVE;
    job_state[JOB_IDX_COUNTDOWN].delta_t = 0.1*ONE_SECOND;
    job_state[JOB_IDX_COUNTDOWN].t_next = wallclock + job_state[JOB_IDX_COUNTDOWN].delta_t;
    job_state[JOB_IDX_COUNTDOWN].event_handler = handleCountDown;

    // Alarm
    job_state[JOB_IDX_ALARM].active = JOB_INACTIVE;
    job_state[JOB_IDX_ALARM].delta_t = 0.04*ONE_SECOND;
    job_state[JOB_IDX_ALARM].t_next = wallclock + job_state[JOB_IDX_ALARM].delta_t;
    job_state[JOB_IDX_ALARM].event_handler = handleAlarm;
}

// Job handler to be called in main loop
void handleJobs(void){
    for(uint8_t i = 0; i < NR_JOBS; i++){
        JobState *js = &job_state[i];
        if(js->active){
            // setLEDArduino();
            // clrLEDArduino();
            if(wallclock >= js->t_next){
                js->t_next = js->t_next + js->delta_t;
                js->event_handler();
            }
        }
    }
}


// Init countdown state
void initCountDownState(void){
    count_down_state.period_idx = 2;  // 20 seconds
    count_down_state.period = timer_period[count_down_state.period_idx];  // 20 seconds
}

// Fading
void initFadeStates(void){
    fade_state_main.period = 0.3*ONE_SECOND;
    fade_state_main.start = wallclock;
    fade_state_main.slope = SLOPE_FALLING;
    fade_state_main.brightness_divider = 1;

    fade_state_breath.period = 0.5*ONE_SECOND;
    fade_state_breath.start = wallclock;
    fade_state_breath.slope = SLOPE_RISING;
    fade_state_breath.brightness_divider = 1;
}

void handleFadeMain(void){
    // Reset start time if period is over
    if(wallclock >= fade_state_main.start + fade_state_main.period){
        // fade_state_main.start = wallclock;
        uint32_t nr_periods = (wallclock - fade_state_main.start)/fade_state_main.period;
        fade_state_main.start += nr_periods*fade_state_main.period;
    }
    // Get pulse width index
    uint8_t pw_idx = ((NR_PWM_LEVELS - 1)*(wallclock - fade_state_main.start))
                     /fade_state_main.period;
    // Invert it if the slope is falling
    if(fade_state_main.slope){
        pw_idx = NR_PWM_LEVELS - 1 - pw_idx;
    }
    // Set PWM output
    setPWMain(pwm_level_16bit[pw_idx]/fade_state_main.brightness_divider);
}

void handleFadeBreath(void){
    // Reset start time if period is over
    if(wallclock >= fade_state_breath.start + fade_state_breath.period){
        // fade_state_breath.start = wallclock;
        uint32_t nr_periods = (wallclock - fade_state_breath.start)/fade_state_breath.period;
        fade_state_breath.start += nr_periods*fade_state_breath.period;
        // Invert slope
        fade_state_breath.slope = !fade_state_breath.slope;
    }
    // Get pulse width index
    uint8_t pw_idx = ((NR_PWM_LEVELS - 1)*(wallclock - fade_state_breath.start))
                     /fade_state_breath.period;
    // Invert it if the slope is falling
    if(fade_state_breath.slope){
        pw_idx = NR_PWM_LEVELS - 1 - pw_idx;
    }
    // Set PWM output
    setPWBreath(pwm_level_16bit[pw_idx]/fade_state_breath.brightness_divider);
}


// Shutdown timer
void handleShutdownTimer(void){
    activateSleepMode();
}

void resetShutdownTimer(void){
    job_state[JOB_IDX_SHUTDOWN_TIMER].t_next = wallclock + job_state[JOB_IDX_SHUTDOWN_TIMER].delta_t;
}

// Set timer
void enterSetTimerMode(void){
    // Start/stop/modify jobs
    job_state[JOB_IDX_FADE_MAIN].active = JOB_INACTIVE;
    job_state[JOB_IDX_FADE_BREATH].active = JOB_ACTIVE;
    job_state[JOB_IDX_SET_TIMER].active = JOB_ACTIVE;
    job_state[JOB_IDX_COUNTDOWN].active = JOB_INACTIVE;
    job_state[JOB_IDX_ALARM].active = JOB_INACTIVE;

    // Assign button handlers
    button_state[BUTTON_MAIN_IDX].event_handler = enterCountDownMode;
    button_state[BUTTON_LEFT_IDX].event_handler = decrementTimer;
    button_state[BUTTON_RIGHT_IDX].event_handler = incrementTimer;

    // Disable main LED
    setPWMain(pwm_level_16bit[0]);

    // Update start time
    count_down_state.start = wallclock;
}

void handleSetTimer(void){
    Time period;
    period.seconds = (count_down_state.period/ONE_SECOND)%60;
    period.minutes = count_down_state.period/ONE_MINUTE;
    count_down_state.display->clear();
    count_down_state.display->drawTime(0, 0, period, 1);
    // count_down_state.display->drawVLine(127 - progess, 17, 15);
    count_down_state.display->show();
}

void incrementTimer(void){
    count_down_state.period_idx = (count_down_state.period_idx < NR_TIMER_PERIODS - 1
                                   ? count_down_state.period_idx + 1
                                   : NR_TIMER_PERIODS - 1);
    count_down_state.period = timer_period[count_down_state.period_idx];
}

void decrementTimer(void){
    count_down_state.period_idx = (count_down_state.period_idx > 0
                                   ? count_down_state.period_idx - 1
                                   : 0);
    count_down_state.period = timer_period[count_down_state.period_idx];
}

// Count-down
void enterCountDownMode(void){
    // Start/stop/modify jobs
    job_state[JOB_IDX_FADE_MAIN].active = JOB_INACTIVE;
    job_state[JOB_IDX_FADE_BREATH].active = JOB_ACTIVE;
    job_state[JOB_IDX_SET_TIMER].active = JOB_INACTIVE;
    job_state[JOB_IDX_COUNTDOWN].active = JOB_ACTIVE;
    job_state[JOB_IDX_ALARM].active = JOB_INACTIVE;

    // Assign button handlers
    button_state[BUTTON_MAIN_IDX].event_handler = enterSetTimerMode;
    button_state[BUTTON_LEFT_IDX].event_handler = doNothing;
    button_state[BUTTON_RIGHT_IDX].event_handler = doNothing;

    // Disable main LED
    setPWMain(pwm_level_16bit[0]);

    // Align fading to timer
    fade_state_breath.start = wallclock;

    // Update start time
    count_down_state.start = wallclock;
}

void handleCountDown(void){
    // Get elapsed minutes and seconds
    uint32_t elapsed = (wallclock - count_down_state.start);
    uint8_t alarm_happend = 0;
    Time remaining;
    if(elapsed < count_down_state.period){
        remaining.seconds = ((count_down_state.period - elapsed)/ONE_SECOND)%60 + 1;
        remaining.minutes = ((count_down_state.period - elapsed)/(ONE_MINUTE));
    } else {
        remaining.seconds = 0;
        remaining.minutes = 0;
        alarm_happend = 1;
    }
    uint8_t progess = elapsed > count_down_state.period ? 127 : ((count_down_state.period - elapsed)*127)/count_down_state.period;
    count_down_state.display->clear();
    count_down_state.display->drawVLine(127 - progess, 17, 15);
    count_down_state.display->drawTime(0, 0, remaining, remaining.seconds%2);
    count_down_state.display->show();
    resetShutdownTimer();
    if(alarm_happend) enterAlarmMode();
}

// Alarm
void enterAlarmMode(void){
    // Start/stop/modify jobs
    job_state[JOB_IDX_FADE_MAIN].active = JOB_ACTIVE;
    job_state[JOB_IDX_FADE_BREATH].active = JOB_ACTIVE;
    job_state[JOB_IDX_SET_TIMER].active = JOB_INACTIVE;
    job_state[JOB_IDX_COUNTDOWN].active = JOB_INACTIVE;
    job_state[JOB_IDX_ALARM].active = JOB_ACTIVE;

    // Assign button handlers
    button_state[BUTTON_MAIN_IDX].event_handler = enterSetTimerMode;
    button_state[BUTTON_LEFT_IDX].event_handler = enterSetTimerMode;
    button_state[BUTTON_RIGHT_IDX].event_handler = enterSetTimerMode;

    // Align fading to timer
    fade_state_main.start = wallclock;
}

void handleAlarm(void){
    count_down_state.display->clear();
    count_down_state.display->drawRandomPattern();
    count_down_state.display->show();
}
