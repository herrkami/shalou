#include "jikan_display.h"

uint32_t random_seed;

void Display::drawNumber(uint8_t pos_x, uint8_t pos_y, uint8_t number){
    // Draw it bit by bit
    for(uint8_t y = 0; y < NUMBER_HEIGHT; y++){
        for(uint8_t x = 0; x < NUMBER_WIDTH; x++){
            uint8_t bit = (numbers[number*7 + x] >> y) & 0x01;
            this->drawPixel(pos_x + x, pos_y + y, bit);
        }
    }
}

void Display::drawNumberDoubleSize(uint8_t pos_x, uint8_t pos_y, uint8_t number){
    // Draw it bit by bit
    for(uint8_t y = 0; y < 2*NUMBER_HEIGHT; y++){
        for(uint8_t x = 0; x < 2*NUMBER_WIDTH; x++){
            uint8_t bit = (numbers[number*7 + x/2] >> y/2) & 0x01;
            this->drawPixel(pos_x + x, pos_y + y, bit);
        }
    }
}

void Display::drawRandomPattern(void){
    // Draw it bit by bit
    for(uint8_t x = 0; x < SSD1306_WIDTH; x++){
        for(uint8_t y = 0; y < SSD1306_HEIGHT; y++){
            uint16_t content = TCNT1;
            this->drawPixel(x, y, (content >> y%16) & 0x01);
        }
    }
}

void Display::drawTime(uint8_t pos_x, uint8_t pos_y, Time time, uint8_t colon){
    this->drawNumberDoubleSize(pos_x + 0*16, pos_y, time.minutes/10);
    this->drawNumberDoubleSize(pos_x + 1*16, pos_y, time.minutes%10);
    if(colon){
        this->drawNumberDoubleSize(pos_x + 2*16, pos_y, 10);
    } else {
        this->drawNumberDoubleSize(pos_x + 2*16, pos_y, 11);
    }
    this->drawNumberDoubleSize(pos_x + 3*16, pos_y, time.seconds/10);
    this->drawNumberDoubleSize(pos_x + 4*16, pos_y, time.seconds%10);
}
