/*
 * Minimal FT3269/FT3x68 bring-up driver for ESP-IDF v6 i2c_master.
 *
 * The register map follows the common FocalTech FT3267/FT3268/FT5x06 layout
 * used by TouchLib and ft3x68-rs. This local driver keeps the first-board test
 * independent from esp_lcd_panel_io_i2c so raw register evidence is clear.
 */

#include "ft3269_touch.h"

#include <stdlib.h>

#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ft3269";

#define FT_REG_TOUCH_POINTS 0x02
#define FT_REG_TOUCH1_XH 0x03
#define FT_REG_CHIP_ID 0xA3
#define FT_REG_POWER_MODE 0xA5

#define FT_TOUCH_RECORD_BYTES 6
#define FT_POWER_ACTIVE 0x00

struct ft3269_touch_dev_t {
    i2c_master_dev_handle_t i2c_dev;
    uint8_t address;
    int timeout_ms;
    gpio_num_t reset_gpio;
    int reset_active_level;
};

static esp_err_t ft3269_read_bytes(ft3269_touch_handle_t touch, uint8_t reg, uint8_t *data, size_t len)
{
    if (!touch || !data || len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    return i2c_master_transmit_receive(touch->i2c_dev, &reg, 1, data, len, touch->timeout_ms);
}

esp_err_t ft3269_touch_create(i2c_master_bus_handle_t bus,
                              uint8_t address,
                              uint32_t scl_speed_hz,
                              int timeout_ms,
                              gpio_num_t reset_gpio,
                              int reset_active_level,
                              ft3269_touch_handle_t *out_touch)
{
    if (!bus || !out_touch) {
        return ESP_ERR_INVALID_ARG;
    }

    ft3269_touch_handle_t touch = calloc(1, sizeof(*touch));
    if (!touch) {
        return ESP_ERR_NO_MEM;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = scl_speed_hz,
    };
    esp_err_t err = i2c_master_bus_add_device(bus, &dev_cfg, &touch->i2c_dev);
    if (err != ESP_OK) {
        free(touch);
        return err;
    }

    touch->address = address;
    touch->timeout_ms = timeout_ms;
    touch->reset_gpio = reset_gpio;
    touch->reset_active_level = reset_active_level;
    *out_touch = touch;
    return ESP_OK;
}

void ft3269_touch_delete(ft3269_touch_handle_t touch)
{
    if (!touch) {
        return;
    }
    if (touch->i2c_dev) {
        i2c_master_bus_rm_device(touch->i2c_dev);
    }
    free(touch);
}

esp_err_t ft3269_touch_init(ft3269_touch_handle_t touch)
{
    if (!touch) {
        return ESP_ERR_INVALID_ARG;
    }

    if (touch->reset_gpio != GPIO_NUM_NC) {
        const gpio_config_t rst_cfg = {
            .pin_bit_mask = 1ULL << touch->reset_gpio,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_RETURN_ON_ERROR(gpio_config(&rst_cfg), TAG, "reset gpio config failed");
        ESP_RETURN_ON_ERROR(gpio_set_level(touch->reset_gpio, !touch->reset_active_level), TAG, "reset precharge failed");
        vTaskDelay(pdMS_TO_TICKS(2));
        ESP_RETURN_ON_ERROR(gpio_set_level(touch->reset_gpio, touch->reset_active_level), TAG, "reset assert failed");
        vTaskDelay(pdMS_TO_TICKS(20));
        ESP_RETURN_ON_ERROR(gpio_set_level(touch->reset_gpio, !touch->reset_active_level), TAG, "reset release failed");
        vTaskDelay(pdMS_TO_TICKS(300));
    }

    uint8_t chip_id = 0;
    esp_err_t err = ft3269_touch_probe(touch, &chip_id);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "probe failed after reset (check SDA/SCL/RST wiring)");
        return err;
    }
    ESP_LOGI(TAG, "chip id 0x%02x", chip_id);

    err = ft3269_touch_write_register(touch, FT_REG_POWER_MODE, FT_POWER_ACTIVE);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "power mode write ignored: %s", esp_err_to_name(err));
    }

    vTaskDelay(pdMS_TO_TICKS(50));
    return ESP_OK;
}

esp_err_t ft3269_touch_probe(ft3269_touch_handle_t touch, uint8_t *chip_id)
{
    if (!touch || !chip_id) {
        return ESP_ERR_INVALID_ARG;
    }
    return ft3269_touch_read_register(touch, FT_REG_CHIP_ID, chip_id);
}

esp_err_t ft3269_touch_read_register(ft3269_touch_handle_t touch, uint8_t reg, uint8_t *value)
{
    return ft3269_read_bytes(touch, reg, value, 1);
}

esp_err_t ft3269_touch_write_register(ft3269_touch_handle_t touch, uint8_t reg, uint8_t value)
{
    if (!touch) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t payload[2] = {reg, value};
    return i2c_master_transmit(touch->i2c_dev, payload, sizeof(payload), touch->timeout_ms);
}

esp_err_t ft3269_touch_read_points(ft3269_touch_handle_t touch,
                                   ft3269_touch_point_t *points,
                                   uint8_t max_points,
                                   uint8_t *point_count)
{
    if (!touch || !point_count || (max_points > 0 && !points)) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t raw_count = 0;
    esp_err_t err = ft3269_touch_read_register(touch, FT_REG_TOUCH_POINTS, &raw_count);
    if (err != ESP_OK) {
        return err;
    }

    uint8_t count = raw_count & 0x0F;
    if (count == 0 || count > 5 || count > max_points) {
        *point_count = 0;
        return ESP_OK;
    }

    uint8_t raw[FT_TOUCH_RECORD_BYTES * 5] = {0};
    err = ft3269_read_bytes(touch, FT_REG_TOUCH1_XH, raw, FT_TOUCH_RECORD_BYTES * count);
    if (err != ESP_OK) {
        return err;
    }

    for (uint8_t i = 0; i < count; ++i) {
        uint8_t *record = &raw[i * FT_TOUCH_RECORD_BYTES];
        points[i].event = record[0] >> 6;
        points[i].x = ((uint16_t)(record[0] & 0x0F) << 8) | record[1];
        points[i].id = record[2] >> 4;
        points[i].y = ((uint16_t)(record[2] & 0x0F) << 8) | record[3];
    }

    *point_count = count;
    return ESP_OK;
}
