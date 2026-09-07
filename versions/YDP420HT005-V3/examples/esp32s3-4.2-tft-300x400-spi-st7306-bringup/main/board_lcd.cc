#include "board_lcd.h"

ST7306_LCD_Color4 lcd(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN, SPI2_HOST);

void board_lcd_init(void) {
    lcd.initialize();
    lcd.High_Power_Mode();
    lcd.display_on(true);
    lcd.display_Inversion(false);
    lcd.clearDisplay();
}

void board_lcd_fill_rect(int x, int y, int w, int h, uint16_t color) {
    for (int yy = y; yy < y + h; ++yy) {
        for (int xx = x; xx < x + w; ++xx) {
            if (xx >= 0 && yy >= 0 && xx < LCD_W && yy < LCD_H) {
                lcd.writePoint((uint16_t)xx, (uint16_t)yy, color);
            }
        }
    }
}

void board_lcd_fill_screen(uint16_t color) {
    board_lcd_fill_rect(0, 0, LCD_W, LCD_H, color);
}
