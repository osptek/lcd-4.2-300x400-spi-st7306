#include "board_lcd_demo.h"

#include "board_color.h"
#include "board_config.h"
#include "board_lcd.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "lcd_demo";

enum DemoPage {
    PAGE_SOLID_BLACK = 0,
    PAGE_SOLID_WHITE,
    PAGE_SOLID_CYAN,
    PAGE_SOLID_RED,
    PAGE_COLOR_BARS,
    PAGE_CHECKER_SMALL,
    PAGE_CHECKER_LARGE,
    PAGE_STRIPES,
    kTotalPages
};

static void draw_color_bars(void) {
    const uint16_t colors[4] = { C_WHITE, C_RED, C_BLACK, C_CYAN };
    const int bar_h = LCD_H / 4;

    for (int i = 0; i < 4; ++i) {
        const int y0 = i * bar_h;
        const int y1 = (i == 3) ? LCD_H : (y0 + bar_h);
        board_lcd_fill_rect(0, y0, LCD_W, y1 - y0, colors[i]);
    }
}

static void draw_checker(int cell) {
    if (cell < 1) {
        cell = 1;
    }
    for (int y = 0; y < LCD_H; ++y) {
        const int row = (y / cell) & 1;
        for (int x = 0; x < LCD_W; ++x) {
            const int col = (x / cell) & 1;
            lcd.writePoint((uint16_t)x, (uint16_t)y, (row ^ col) ? C_BLACK : C_WHITE);
        }
    }
}

static void draw_stripes(int stripe_h) {
    if (stripe_h < 1) {
        stripe_h = 1;
    }
    for (int y = 0; y < LCD_H; ++y) {
        const uint16_t color = ((y / stripe_h) & 1) ? C_BLACK : C_WHITE;
        board_lcd_fill_rect(0, y, LCD_W, 1, color);
    }
}

static void render_page(int page) {
    lcd.clearDisplay();

    switch (page) {
        case PAGE_SOLID_BLACK:
            board_lcd_fill_screen(C_BLACK);
            break;
        case PAGE_SOLID_WHITE:
            board_lcd_fill_screen(C_WHITE);
            break;
        case PAGE_SOLID_CYAN:
            board_lcd_fill_screen(C_CYAN);
            break;
        case PAGE_SOLID_RED:
            board_lcd_fill_screen(C_RED);
            break;
        case PAGE_COLOR_BARS:
            draw_color_bars();
            break;
        case PAGE_CHECKER_SMALL:
            draw_checker(8);
            break;
        case PAGE_CHECKER_LARGE:
            draw_checker(40);
            break;
        case PAGE_STRIPES:
            draw_stripes(16);
            break;
        default:
            break;
    }

    lcd.display();
    ESP_LOGI(TAG, "demo page %d / %d", page, kTotalPages - 1);
}

void board_lcd_demo_start(void) {
    ESP_LOGI(TAG, "LCD pattern demo looping");
    int page = 0;
    while (true) {
        render_page(page);
        vTaskDelay(pdMS_TO_TICKS(BOARD_LCD_DEMO_DWELL_MS));
        page = (page + 1) % kTotalPages;
    }
}
