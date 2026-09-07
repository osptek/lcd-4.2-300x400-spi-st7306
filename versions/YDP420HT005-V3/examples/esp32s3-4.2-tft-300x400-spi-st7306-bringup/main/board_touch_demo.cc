#include "board_touch_demo.h"

#include <stdio.h>

#include "board_color.h"
#include "board_lcd.h"
#include "board_touch_priv.h"
#include "board_ui_draw.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "st7306_color4.h"

static const char *TAG = "touch_demo";

static void map_touch_point(const ft3269_touch_point_t *in, uint16_t *x, uint16_t *y) {
    *x = in->x;
    *y = in->y;
    if (*x >= LCD_W) {
        *x = LCD_W - 1;
    }
    if (*y >= LCD_H) {
        *y = LCD_H - 1;
    }
}

static void draw_touch_legend(void) {
    const uint16_t colors[4] = { C_WHITE, C_CYAN, C_RED, C_BLACK };
    const int h = 8;
    const int y = LCD_H - h;
    const int sw = LCD_W / 4;
    for (int i = 0; i < 4; ++i) {
        board_lcd_fill_rect(i * sw, y, sw, h, colors[i]);
    }
}

static void draw_axes(void) {
    const int ox = 20;
    const int oy = LCD_H - 20;
    const int tick = 50;
    const int tick_len = 4;

    board_lcd_fill_rect(ox, 8, 1, oy - 8, C_BLACK);
    board_lcd_fill_rect(ox, oy, LCD_W - ox - 4, 1, C_BLACK);
    board_ui_draw_text(LCD_W - 14, oy - 16, "X", 1, C_BLACK);
    board_ui_draw_text(ox + 4, 8, "Y", 1, C_BLACK);

    for (int x = ox + tick; x < LCD_W - 8; x += tick) {
        board_lcd_fill_rect(x, oy - tick_len, 1, tick_len, C_BLACK);
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", x);
        board_ui_draw_text(x - 8, oy + 2, buf, 1, C_BLACK);
    }
    for (int y = oy - tick; y > 16; y -= tick) {
        board_lcd_fill_rect(ox, y, tick_len, 1, C_BLACK);
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", y);
        board_ui_draw_text(2, y - 4, buf, 1, C_BLACK);
    }
}

static void draw_touch_cross(uint16_t cx, uint16_t cy, uint16_t color) {
    const int t = 2;
    int hy = (cy > t / 2) ? (cy - t / 2) : 0;
    board_lcd_fill_rect(0, hy, LCD_W, t, color);
    int vx = (cx > t / 2) ? (cx - t / 2) : 0;
    board_lcd_fill_rect(vx, 0, t, LCD_H, color);
    board_lcd_fill_rect((int)cx - 2, (int)cy - 2, 5, 5, C_RED);
}

static void draw_touch_ui(uint16_t sx, uint16_t sy, uint8_t count, const uint16_t *xs, const uint16_t *ys) {
    lcd.clearDisplay();
    draw_axes();
    for (uint8_t i = 0; i < count; ++i) {
        draw_touch_cross(xs[i], ys[i], (i == 0) ? C_BLACK : C_CYAN);
    }
    draw_touch_legend();

    char buf[16];
    snprintf(buf, sizeof(buf), "%u:%u", sx, sy);
    board_ui_draw_text(28, 8, buf, 2, C_BLACK);
    lcd.display();
}

static void touch_poll_task(void *arg) {
    (void)arg;
    ft3269_touch_point_t points[5];
    ESP_LOGI(TAG, "Touch demo started");

    lcd.clearDisplay();
    draw_axes();
    draw_touch_legend();
    lcd.display();

    while (true) {
        uint8_t count = 0;
        esp_err_t err = board_touch_read_raw_points(points, 5, &count);
        if (err != ESP_OK) {
            static TickType_t s_last_warn;
            TickType_t now = xTaskGetTickCount();
            if (now - s_last_warn > pdMS_TO_TICKS(2000)) {
                ESP_LOGW(TAG, "read_points: %s", esp_err_to_name(err));
                s_last_warn = now;
            }
            vTaskDelay(pdMS_TO_TICKS(50));
            continue;
        }

        if (count > 0) {
            uint16_t xs[5] = {};
            uint16_t ys[5] = {};
            for (uint8_t i = 0; i < count; ++i) {
                map_touch_point(&points[i], &xs[i], &ys[i]);
                ESP_LOGI(TAG, "touch[%u] raw(%u,%u) -> screen(%u,%u)",
                         i, points[i].x, points[i].y, xs[i], ys[i]);
            }
            draw_touch_ui(xs[0], ys[0], count, xs, ys);
        }

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void board_touch_demo_start(void) {
    xTaskCreate(touch_poll_task, "touch_poll", 4096, nullptr, 5, nullptr);
}
