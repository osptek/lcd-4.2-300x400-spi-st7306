#pragma once

#include <cstdint>
#include "st7306_color4.h"

extern ST7306_LCD_Color4 lcd;

void board_lcd_init(void);
void board_lcd_fill_rect(int x, int y, int w, int h, uint16_t color);
void board_lcd_fill_screen(uint16_t color);
