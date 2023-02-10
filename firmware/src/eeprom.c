#include "eeprom.h"


void eepromInit(void){
  // Allow read and write
  CLEARBIT(EECR, EEPM1);
  CLEARBIT(EECR, EEPM0);

  // Enable EEPROM and disable EEPE to make sure, it doesn't block
  // SETBIT(EECR, EEMPE);
  CLEARBIT(EECR, EEPE);
}

void eepromWrite(uint16_t address, uint8_t data){
  // Wait for completion of previous write
  while(EECR & (1 << EEPE));
  // Set up address and data registers
  EEAR = address;
  EEDR = data;
  // Write logical one to EEMPE
  SETBIT(EECR, EEMPE);
  // Start eeprom write by setting EEPE
  // EECR |= (1 << EEPE);
  SETBIT(EECR, EEPE);
}

uint8_t eepromRead(uint16_t address){
  // Wait for completion of previous write
  while(EECR & (1 << EEPE));
  // Set up address register
  EEAR = address;
  // Start eeprom read by writing EERE
  SETBIT(EECR, EERE);
  // Return data from data register
  return (uint8_t)EEDR;
}
