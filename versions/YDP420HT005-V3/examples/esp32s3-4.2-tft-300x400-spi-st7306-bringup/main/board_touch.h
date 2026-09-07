#pragma once

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

void board_touch_prepare_pins(void);
esp_err_t board_touch_init(void);

#ifdef __cplusplus
}
#endif
