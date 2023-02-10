#include "jikan_io.h"
#include "avr035.h"

uint16_t pw_main;
uint16_t pw_breath;

// Pin initializations
void initButtons(void){
    // Set as inputs
    CLEARBITMASK(BUTTON_DDR_REG, BUTTON_BITMASK);
    // Activate internal pull-up resistor
    SETBITMASK(BUTTON_PORT_REG, BUTTON_BITMASK);
}

void initChargingState(void){
    // Set as inputs
    CLEARBIT(CHARGING_STATE_DDR_REG, CHARGING_STATE);
    // Activate internal pull-up resistor
    SETBIT(CHARGING_STATE_PORT_REG, CHARGING_STATE);
}

void initLEDMain(void){
    // Set as output
    SETBIT(LED_DDR_REG, LED_MAIN);
    // Initialize low
    SETBIT(LED_PORT_REG, LED_MAIN);
}

void initLEDBreath(void){
  // Set as output
  SETBIT(LED_DDR_REG, LED_BREATH);
  // Initialize low
  CLEARBIT(LED_PORT_REG, LED_BREATH);
}

void initLEDArduino(void){
  // Set as output
  SETBIT(LED_ARDUINO_DDR_REG, LED_ARDUINO);
  // Initialize low
  CLEARBIT(LED_ARDUINO_PORT_REG, LED_ARDUINO);
}

void initOLEDPower(void){
  // Set as output
  SETBITMASK(OLED_POWER_DDR_REG, OLED_POWER_MASK);
  // Initialize low
  CLEARBITMASK(OLED_POWER_PORT_REG, OLED_POWER_MASK);
}

void initPWM(void){
    // 16 bit normal PWM mode
    // Prescaler to 1 -> 16 MHz / 65536 / 1 = 244.140625 Hz
    // OC1A: LED_BREATH
    // OC1B: LED_MAIN (inverted)
    TCCR1A = (
        (1 << COM1A1) |\
        (1 << COM1B1) |\
        (1 << COM1B0));
    TCCR1B = ((1<<WGM13) | (1 << CS10));
    ICR1 = 0xffff;

    pw_main = 1;
    pw_breath = 1;
}

void initIOs(void){
    // Buttons
    initButtons();
    initChargingState();
    // LEDs
    initLEDMain();
    initLEDBreath();
    initLEDArduino();
    initOLEDPower();
}


// Button state getters
uint8_t getButtonMain(void){
  return !CHECKBIT(BUTTON_PIN_REG, BUTTON_MAIN);
}

uint8_t getButtonLeft(void){
  return !CHECKBIT(BUTTON_PIN_REG, BUTTON_LEFT);
}

uint8_t getButtonRight(void){
  return !CHECKBIT(BUTTON_PIN_REG, BUTTON_RIGHT);
}

// Charging state getter
uint8_t getChargingState(void){
  return !CHECKBIT(CHARGING_STATE_PIN_REG, CHARGING_STATE);
}

// Output setters
void setLEDMain(void){
  CLEARBIT(LED_PORT_REG, LED_MAIN);
}

void clrLEDMain(void){
  SETBIT(LED_PORT_REG, LED_MAIN);
}

void setLEDBreath(void){
  SETBIT(LED_PORT_REG, LED_BREATH);
}

void clrLEDBreath(void){
  CLEARBIT(LED_PORT_REG, LED_BREATH);
}

void setLEDArduino(void){
  SETBIT(LED_ARDUINO_PORT_REG, LED_ARDUINO);
}

void clrLEDArduino(void){
  CLEARBIT(LED_ARDUINO_PORT_REG, LED_ARDUINO);
}

void setOLEDPower(void){
  SETBITMASK(OLED_POWER_PORT_REG, OLED_POWER_MASK);
}

void clrOLEDPower(void){
  CLEARBITMASK(OLED_POWER_PORT_REG, OLED_POWER_MASK);
}

// PWM
void setPWBreath(uint16_t pw){
    pw_breath = pw;
    // OCR1AH = (pw >> 8);
    // OCR1AL = (pw & 0xff);
    OCR1A = pw;
}

void setPWMain(uint16_t pw){
    pw_main = pw;
    // OCR1BH = (pw >> 8);
    // OCR1BL = (pw & 0xff);
    OCR1B = pw;
}

void activatePWMLEDs(void){
    setPWMain(pw_main);
    setPWBreath(pw_breath);

}

void deactivatePWMLEDs(void){
    setPWMain(0);
    setPWBreath(0);
}

void decrementBrightnessMain(void){
    pw_main = pw_main>1 ? pw_main/2 : 1;
    setPWMain(pw_main);
}

void incrementBrightnessMain(void){
    pw_main = pw_main<128 ? pw_main*2 : 254;
    setPWMain(pw_main);
}

void decrementBrightnessBreath(void){
    pw_breath = pw_breath>1 ? pw_breath/2 : 1;
    setPWBreath(pw_breath);
}

void incrementBrightnessBreath(void){
    pw_breath = pw_breath<128 ? pw_breath*2 : 254;
    setPWBreath(pw_breath);
}
