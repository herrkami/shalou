#include "jikan_time.h"

volatile uint32_t _wallclock;
volatile uint32_t wallclock;

void initWallClock(void){
    // Execute interrupt with 16 MHz / 1024 / 125 = 125 Hz
    // Execute interrupt with 16 MHz / 256 / 125 = 500 Hz
    // Init wall clock counter
    _wallclock = 0;
    wallclock = _wallclock;
    // Set counter 0 to CTC mode
    TCCR0A = (1 << WGM01);
    // // Set prescaler to 1024
    // TCCR0B = ((1 << CS02) | (1 << CS00));
    // Set prescaler to 256
    TCCR0B = (1 << CS02);
    // Set reset TOP to 125
    OCR0A = 124;
    // Enable interrupt on compare match
    TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect){
    _wallclock++;
    wallclock = _wallclock;
    // setLEDArduino();
    // clrLEDArduino();
}
