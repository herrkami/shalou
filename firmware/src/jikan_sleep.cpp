#include "jikan_sleep.h"

void configureSleepMode(void){
    // Configure sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // SMCR = (1 << SM1);
}

void activateSleepMode(void){
    // Disable eveything
    deactivatePWMLEDs();
    // Give the PWM engine time to set the pin to 0
    _delay_ms(10);

    clrLEDArduino();
    clrLEDMain();
    clrLEDBreath();
    clrOLEDPower();

    // Enable sleeping
    sleep_enable();
    // Make sure interrupts are active so we wake up again
    sei();
    // Configure button main interrupt on low level (default option)
    EIMSK = (1 << INT1);
    // Zzzz
    sleep_cpu();
}

ISR(INT1_vect){
    // Main button has been pressed
    // Clear sleep causing interrupt
    EIMSK = 0;

    // Disable sleeping
    sleep_disable();

    // Reactivate PWM
    activatePWMLEDs();

    // Reset the shutdown timer
    resetShutdownTimer();

    // Enable display
    setOLEDPower();
    _delay_ms(100);
    SSD1306 oled;

    // Enter set timer mode
    enterSetTimerMode();
}
