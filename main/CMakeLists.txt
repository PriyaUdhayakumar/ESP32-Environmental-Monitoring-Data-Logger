idf_component_register(SRCS "app_main.c" "i2c.c" "bme280.c" "sensor.c"  "rtc.c" "sdcard.c" "logger.c"  "led.c"   "uart.c"
                       PRIV_REQUIRES esp_driver_gpio esp_driver_i2c esp_driver_spi esp_driver_sdspi fatfs sdmmc
                       INCLUDE_DIRS "")
