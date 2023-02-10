#include <avr/io.h>

#include <util/delay.h>

// #define F_CPU 16000000UL

#include "jikan_io.h"
#include "jikan_sleep.h"
#include "jikan_time.h"
#include "jikan_ui.h"
#include "jikan_jobs.h"
#include "jikan_display.h"
// #include "numbers.h"
#include "framebuffer.h"


int main(void) {

    // Initialize IOs
    initIOs();
    // Initialize PWM
    initPWM();
    // Configure sleep mode
    configureSleepMode();
    // Init wall clock
    initWallClock();
    // Init user interface
    initUI();
    // Init jobs scheduling
    initJobs();
    // Init LED faders
    initFadeStates();
    // Init countdown state
    initCountDownState();
    // Enter initial mode
    enterSetTimerMode();

    // Switch on OLED display and wait for it to start up
    setOLEDPower();
    _delay_ms(200);
    // Init display
    Display display;

    // Init count-down
    count_down_state.display = &display;


    uint8_t loop_ctr = 0;

    // Enable interrupts
    sei();

    for(;;){

        // Indicate charging
        if(getChargingState()){
            setLEDArduino();
        } else {
            clrLEDArduino();
        }

        handleJobs();

        loop_ctr = loop_ctr < 255? loop_ctr + 1 : 0;
    }
    return 0;
}
