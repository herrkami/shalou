#ifndef JIKAN_IO_H
#define JIKAN_IO_H

#include <avr/io.h>

// Buttons
#define BUTTON_PORT_REG PORTD
#define BUTTON_DDR_REG DDRD
#define BUTTON_PIN_REG PIND

#define BUTTON_MAIN PD3
#define BUTTON_LEFT PD4
#define BUTTON_RIGHT PD5

#define BUTTON_BITMASK ((1 << (BUTTON_MAIN)) |\
                        (1 << (BUTTON_LEFT)) |\
                        (1 << (BUTTON_RIGHT)))

// Charing indicator
#define CHARGING_STATE_PORT_REG PORTD
#define CHARGING_STATE_DDR_REG DDRD
#define CHARGING_STATE_PIN_REG PIND

#define CHARGING_STATE PD2

// LEDs
#define LED_PORT_REG PORTB
#define LED_DDR_REG DDRB
#define LED_PIN_REG PINB

#define LED_MAIN PB2
#define LED_BREATH PB1

#define LED_ARDUINO_PORT_REG PORTB
#define LED_ARDUINO_DDR_REG DDRB
#define LED_ARDUINO_PIN_REG PINB

#define LED_ARDUINO PB5

// OLED power pins
#define OLED_POWER_PORT_REG PORTC
#define OLED_POWER_DDR_REG DDRC
#define OLED_POWER_PIN_REG PINC

#define OLED_POWER_MASK ((1 << PC0) | (1 << PC1))

// 8 bit PWM levels on a log scale
#define NR_PWM_LEVELS 64

const uint16_t pwm_level_16bit[NR_PWM_LEVELS] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002,
    0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0009, 0x000b, 0x000d,
    0x0010, 0x0014, 0x0019, 0x001f, 0x0026, 0x0030, 0x003b, 0x0049,
    0x005a, 0x006f, 0x008a, 0x00aa, 0x00d3, 0x0105, 0x0142, 0x018f,
    0x01ed, 0x0262, 0x02f2, 0x03a5, 0x0482, 0x0594, 0x06e6, 0x0888,
    0x0a8d, 0x0d0d, 0x1025, 0x13f8, 0x18b3, 0x1e8c, 0x25c9, 0x2ebc,
    0x39ce, 0x477f, 0x586f, 0x6d61, 0x874a, 0xa755, 0xcef8, 0xffff};

extern uint16_t pw_main;
extern uint16_t pw_breath;

void initButtons(void);
void initChargingState(void);
void initLEDMain(void);
void initLEDBreath(void);
void initLEDArduino(void);
void initOLEDPower(void);

void initPWM(void);

void initIOs(void);

uint8_t getButtonMain(void);
uint8_t getButtonLeft(void);
uint8_t getButtonRight(void);
uint8_t getChargingState(void);

void setLEDMain(void);
void clrLEDMain(void);
void setLEDBreath(void);
void clrLEDBreath(void);
void setLEDArduino(void);
void clrLEDArduino(void);
void setOLEDPower(void);
void clrOLEDPower(void);

void activatePWMLEDs(void);
void deactivatePWMLEDs(void);
void setPWBreath(uint16_t pw);
void setPWMain(uint16_t pw);
void decrementBrightnessMain(void);
void incrementBrightnessMain(void);
void decrementBrightnessBreath(void);
void incrementBrightnessBreath(void);


#endif /* JIKAN_IO_H */
