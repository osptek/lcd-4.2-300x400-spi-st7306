#pragma once

#include "driver/gpio.h"

/* 启动哪个 demo：只跑其中一个，不会串着进下一个 */
#define BOARD_DEMO_PATTERN        0
#define BOARD_DEMO_TOUCH          1

#ifndef BOARD_DEMO
#define BOARD_DEMO                BOARD_DEMO_TOUCH
#endif

#define BOARD_LCD_DEMO_DWELL_MS   2000

// 板级引脚方案：改这一处即可切换整套接线
//   1 = 新板（LCD 9~14 + FT3269 39~42）
//   0 = 转接板（原先 DC=40 / RST=41 / CS=38 / SCLK=39 / MOSI=42 / TE=47）
#ifndef BOARD_PINSET
#define BOARD_PINSET 1
#endif

#if BOARD_PINSET == 1
/* ---- LCD ST7306 ---- */
#define BOARD_PIN_LCD_CS          GPIO_NUM_9
#define BOARD_PIN_LCD_RST         GPIO_NUM_10
#define BOARD_PIN_LCD_TE          GPIO_NUM_11
#define BOARD_PIN_LCD_CLK         GPIO_NUM_12
#define BOARD_PIN_LCD_MOSI        GPIO_NUM_13
#define BOARD_PIN_LCD_RS          GPIO_NUM_14

/* 触摸 I2C FT3269 @ 0x38 */
#define BOARD_PIN_TOUCH_SDA       GPIO_NUM_39
#define BOARD_PIN_TOUCH_SCL       GPIO_NUM_40
#define BOARD_PIN_TOUCH_RST       GPIO_NUM_41
#define BOARD_PIN_TOUCH_INT       GPIO_NUM_42
#define BOARD_TOUCH_I2C_ADDR      0x38
#else
/* ---- LCD ST7306（转接板）---- */
#define BOARD_PIN_LCD_CS          GPIO_NUM_38
#define BOARD_PIN_LCD_RST         GPIO_NUM_41
#define BOARD_PIN_LCD_TE          GPIO_NUM_47
#define BOARD_PIN_LCD_CLK         GPIO_NUM_39
#define BOARD_PIN_LCD_MOSI        GPIO_NUM_42
#define BOARD_PIN_LCD_RS          GPIO_NUM_40

/* 触摸 I2C FT3269 @ 0x38（转接板预留，当前未接） */
#define BOARD_PIN_TOUCH_SDA       GPIO_NUM_39
#define BOARD_PIN_TOUCH_SCL       GPIO_NUM_38
#define BOARD_PIN_TOUCH_RST       GPIO_NUM_40
#define BOARD_PIN_TOUCH_INT       GPIO_NUM_47
#define BOARD_TOUCH_I2C_ADDR      0x38
#endif

/* 兼容旧宏：LCD 驱动 / 构造函数仍用这组名字（不要加括号，会和类成员名冲突） */
#define DC_PIN                    BOARD_PIN_LCD_RS    // D/C，丝印 RS
#define RES_PIN                   BOARD_PIN_LCD_RST
#define CS_PIN                    BOARD_PIN_LCD_CS
#define SCLK_PIN                  BOARD_PIN_LCD_CLK
#define SDIN_PIN                  BOARD_PIN_LCD_MOSI
#define TE_PIN                    BOARD_PIN_LCD_TE

#define BOARD_TOUCH_RST_ACTIVE    0
#define BOARD_TOUCH_I2C_HZ        100000
#define BOARD_TOUCH_I2C_TIMEOUT_MS 200
#define BOARD_TOUCH_CROSS_THICK   3
