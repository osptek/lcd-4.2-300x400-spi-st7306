#pragma once

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FT3269_I2C_ADDR 0x38

typedef struct ft3269_touch_dev_t *ft3269_touch_handle_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t id;
    uint8_t event;
} ft3269_touch_point_t;

esp_err_t ft3269_touch_create(i2c_master_bus_handle_t bus,
                              uint8_t address,
                              uint32_t scl_speed_hz,
                              int timeout_ms,
                              gpio_num_t reset_gpio,
                              int reset_active_level,
                              ft3269_touch_handle_t *out_touch);
void ft3269_touch_delete(ft3269_touch_handle_t touch);
esp_err_t ft3269_touch_init(ft3269_touch_handle_t touch);
esp_err_t ft3269_touch_probe(ft3269_touch_handle_t touch, uint8_t *chip_id);
esp_err_t ft3269_touch_read_register(ft3269_touch_handle_t touch, uint8_t reg, uint8_t *value);
esp_err_t ft3269_touch_write_register(ft3269_touch_handle_t touch, uint8_t reg, uint8_t value);
esp_err_t ft3269_touch_read_points(ft3269_touch_handle_t touch,
                                   ft3269_touch_point_t *points,
                                   uint8_t max_points,
                                   uint8_t *point_count);

#ifdef __cplusplus
}
#endif
