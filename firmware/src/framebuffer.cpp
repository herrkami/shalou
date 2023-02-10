/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include "framebuffer.h"
#include "numbers.h"

Framebuffer::Framebuffer() {
    this->clear();
}

void Framebuffer::drawBitmap(const uint8_t *progmem_bitmap, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y) {
    // Width/height in pixels
    uint8_t current_byte;
    uint8_t byte_width = (width + 7)/8;

    for (uint8_t y = 0; y < height; y++) {
        for (uint8_t x = 0; x < width; x++) {
            current_byte = pgm_read_byte(progmem_bitmap + y*byte_width + x/8);
            if (current_byte & (128 >> (x&7))) {
                this->drawPixel(x+pos_x, y+pos_y, 1);
            } else {
                this->drawPixel(x+pos_x, y+pos_y, 0);
            }
        }
    }
}

void Framebuffer::drawBuffer(const uint8_t *progmem_buffer) {
    uint8_t current_byte;

    for (uint8_t y_pos = 0; y_pos < SSD1306_HEIGHT; y_pos++) {
        for (uint8_t x_pos = 0; x_pos < 128; x_pos++) {
            current_byte = pgm_read_byte(progmem_buffer + y_pos*16 + x_pos/8);
            if (current_byte & (128 >> (x_pos&7))) {
                this->drawPixel(x_pos,y_pos,1);
            } else {
                this->drawPixel(x_pos,y_pos,0);
            }
        }
    }
}

void Framebuffer::drawFrameBuffer(uint8_t *buffer) {
    this->oled.sendFramebuffer(buffer);
}

void Framebuffer::drawPixel(uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status) {
    if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT) {
        return;
    }

    if (pixel_status) {
        this->buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
    } else {
        this->buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] &= ~(1 << (pos_y&7));
    }
}

void Framebuffer::drawPixel(uint8_t pos_x, uint8_t pos_y) {
    if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT) {
        return;
    }

    this->buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
}

void Framebuffer::drawVLine(uint8_t x, uint8_t y, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) {
        this->drawPixel(x,i+y);
    }
}

void Framebuffer::drawHLine(uint8_t x, uint8_t y, uint8_t length) {
    for (uint8_t i = 0; i < length; ++i) {
        this->drawPixel(i+x,y);
    }
}

void Framebuffer::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t length = x2 - x1 + 1;
    uint8_t height = y2 - y1;

    this->drawHLine(x1,y1,length);
    this->drawHLine(x1,y2,length);
    this->drawVLine(x1,y1,height);
    this->drawVLine(x2,y1,height);
}

void Framebuffer::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill) {
    if (!fill) {
        this->drawRectangle(x1,y1,x2,y2);
    } else {
        uint8_t length = x2 - x1 + 1;
        uint8_t height = y2 - y1;

        for (int x = 0; x < length; ++x) {
            for (int y = 0; y <= height; ++y) {
                this->drawPixel(x1+x,y+y1);
            }
        }
    }
}

void Framebuffer::clear() {
    for (uint16_t buffer_location = 0; buffer_location < SSD1306_BUFFERSIZE; buffer_location++) {
        this->buffer[buffer_location] = 0x00;
    }
}

void Framebuffer::invert(uint8_t status) {
    this->oled.invert(status);
}

void Framebuffer::show() {
    this->oled.sendFramebuffer(this->buffer);
}
