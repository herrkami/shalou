#include "jikan_ui.h"

ButtonState button_state[NR_BUTTONS];

// Button handlers
void assignButtonHandlers(void){
    button_state[BUTTON_MAIN_IDX].button_handler = getButtonMain;
    button_state[BUTTON_LEFT_IDX].button_handler = getButtonLeft;
    button_state[BUTTON_RIGHT_IDX].button_handler = getButtonRight;
}

void assignButtonEvents(void){
    button_state[BUTTON_MAIN_IDX].event_handler = enterSetTimerMode;
    button_state[BUTTON_LEFT_IDX].event_handler = decrementBrightnessMain;
    button_state[BUTTON_RIGHT_IDX].event_handler = incrementBrightnessMain;
}

void assignButtonHoldEvents(void){
    button_state[BUTTON_MAIN_IDX].hold_event_handler = doNothing;
    button_state[BUTTON_LEFT_IDX].hold_event_handler = activateSleepMode;
    button_state[BUTTON_RIGHT_IDX].hold_event_handler = doNothing;
}

void updateButtonStates(void){
    for(uint8_t i = 0; i < NR_BUTTONS; i++){
        ButtonState *bs = &button_state[i];
        bs->state_previous = bs->state_current;
        bs->state_current = bs->button_handler();
        // Button is pressed
        if(!bs->state_previous && bs->state_current){
            bs->pressed = 1;
            // Reset the shutdown timer if user is using the device
            resetShutdownTimer();
        } else {
            bs->pressed = 0;
        }
        // Button still held
        if(bs->state_previous && bs->state_current){
            // Increment hold counter
            bs->hold_ctr = bs->hold_ctr < HOLD_CTR_MAX ? bs->hold_ctr + 1 : HOLD_CTR_MAX;
            if(bs->hold_ctr == HOLD_CTR_DETECT){
                bs->hold_event_happend = 1;
                bs->hold_event_handler();
            }
        }
        // Button is released
        if(bs->state_previous && !bs->state_current){
            if(!bs->hold_event_happend){
                bs->event_handler();
            }
            bs->hold_ctr = 0;
            bs->hold_event_happend = 0;
        }
    }
}

void initUI(void){
  assignButtonHandlers();
  assignButtonEvents();
  assignButtonHoldEvents();
  updateButtonStates();
  updateButtonStates();
}

// Event handler
void doNothing(void){
  return;
}
