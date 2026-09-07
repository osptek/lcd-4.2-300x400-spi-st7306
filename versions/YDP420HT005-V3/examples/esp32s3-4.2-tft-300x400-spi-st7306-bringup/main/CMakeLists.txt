idf_component_register(
    SRCS 
        "mian.cc"
        "board_lcd.cc"
        "board_ui_draw.cc"
        "board_lcd_demo.cc"
        "board_touch.c"
        "board_touch_demo.cc"
        "ft3269_touch/ft3269_touch.c"
        "st7306/st7306_color4.cc"
                    
    INCLUDE_DIRS 
        "."
        "st7306"
        "ft3269_touch"
    
    REQUIRES driver esp_driver_gpio esp_driver_i2c
)
