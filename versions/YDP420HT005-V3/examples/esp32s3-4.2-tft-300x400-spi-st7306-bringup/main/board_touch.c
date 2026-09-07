#include "board_touch.h"

#include "board_config.h"
#include "board_touch_priv.h"
#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ft3269_touch.h"

static const char *TAG = "board_touch";

static const uint8_t s_touch_addr_candidates[] = {
    FT3269_I2C_ADDR,
    0x48,
    0x15,
    0x5D,
};

static i2c_master_bus_handle_t s_i2c_bus;
static ft3269_touch_handle_t s_touch;

void board_touch_prepare_pins(void)
{
    const gpio_config_t rst_cfg = {
        .pin_bit_mask = 1ULL << BOARD_PIN_TOUCH_RST,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&rst_cfg);
    gpio_set_level(BOARD_PIN_TOUCH_RST, !BOARD_TOUCH_RST_ACTIVE);

    const gpio_config_t int_cfg = {
        .pin_bit_mask = 1ULL << BOARD_PIN_TOUCH_INT,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&int_cfg);
}

static esp_err_t find_touch_address(i2c_master_bus_handle_t bus, uint8_t *out_addr)
{
    for (size_t i = 0; i < sizeof(s_touch_addr_candidates); ++i) {
        uint8_t addr = s_touch_addr_candidates[i];
        if (i2c_master_probe(bus, addr, BOARD_TOUCH_I2C_TIMEOUT_MS) == ESP_OK) {
            *out_addr = addr;
            ESP_LOGI(TAG, "touch probe hit 0x%02X", addr);
            return ESP_OK;
        }
    }
    ESP_LOGW(TAG, "no touch IC — check SDA(%d)/SCL(%d) and power",
             (int)BOARD_PIN_TOUCH_SDA, (int)BOARD_PIN_TOUCH_SCL);
    return ESP_ERR_NOT_FOUND;
}

esp_err_t board_touch_init(void)
{
    board_touch_prepare_pins();

    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = BOARD_PIN_TOUCH_SDA,
        .scl_io_num = BOARD_PIN_TOUCH_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_RETURN_ON_ERROR(i2c_new_master_bus(&bus_cfg, &s_i2c_bus), TAG, "i2c bus init failed");

    vTaskDelay(pdMS_TO_TICKS(50));

    uint8_t touch_addr = FT3269_I2C_ADDR;
    esp_err_t err = find_touch_address(s_i2c_bus, &touch_addr);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "no touch IC on I2C bus");
        return err;
    }

    ESP_RETURN_ON_ERROR(ft3269_touch_create(s_i2c_bus,
                                            touch_addr,
                                            BOARD_TOUCH_I2C_HZ,
                                            BOARD_TOUCH_I2C_TIMEOUT_MS,
                                            BOARD_PIN_TOUCH_RST,
                                            BOARD_TOUCH_RST_ACTIVE,
                                            &s_touch),
                        TAG,
                        "ft3269 create failed");

    ESP_RETURN_ON_ERROR(ft3269_touch_init(s_touch), TAG, "ft3269 init failed");

    ESP_LOGI(TAG, "Touch ready @ 0x%02X", touch_addr);
    return ESP_OK;
}

esp_err_t board_touch_read_raw_points(ft3269_touch_point_t *points,
                                      uint8_t max_points,
                                      uint8_t *point_count)
{
    if (!s_touch) {
        return ESP_ERR_INVALID_STATE;
    }
    return ft3269_touch_read_points(s_touch, points, max_points, point_count);
}
