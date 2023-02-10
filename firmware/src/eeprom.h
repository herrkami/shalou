#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>

#include "avr035.h"

#define EEPROM_ADR_RULE   0
#define EEPROM_ADR_WIDTH  1
#define EEPROM_ADR_OCTAVE 2
#define EEPROM_ADR_RESET  3

void eepromInit(void);
void eepromWrite(uint16_t address, uint8_t data);
uint8_t eepromRead(uint16_t address);

#endif /* EEPROM_H */
