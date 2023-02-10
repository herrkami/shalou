#ifndef _JIKAN_DIPLAY_H_
#define _JIKAN_DIPLAY_H_

#include "framebuffer.h"
#include "numbers.h"
#include "jikan_io.h"
#include "jikan_time.h"

class Display : public Framebuffer {
public:
    void drawNumber(uint8_t pos_x, uint8_t pos_y, uint8_t number);
    void drawNumberDoubleSize(uint8_t pos_x, uint8_t pos_y, uint8_t number);
    void drawRandomPattern(void);
    void drawTime(uint8_t pos_x, uint8_t pos_y, Time time, uint8_t colon);
private:
};

#endif
