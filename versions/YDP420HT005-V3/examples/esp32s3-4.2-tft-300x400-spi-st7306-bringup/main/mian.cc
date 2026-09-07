#include "board_config.h"
#include "board_lcd.h"
#include "board_lcd_demo.h"
#include "board_touch.h"
#include "board_touch_demo.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "APP";

extern "C" void app_main(void) {
    board_lcd_init();

#if BOARD_DEMO == BOARD_DEMO_TOUCH
    board_touch_prepare_pins();
    vTaskDelay(pdMS_TO_TICKS(200));
    esp_err_t terr = board_touch_init();
    if (terr != ESP_OK) {
        ESP_LOGE(TAG, "touch_init failed: %s", esp_err_to_name(terr));
        return;
    }
    board_touch_demo_start();
#else
    board_lcd_demo_start();
#endif
}
