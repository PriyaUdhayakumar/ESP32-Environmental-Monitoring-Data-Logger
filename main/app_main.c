// Main Program
#include "sensor.h"
#include "rtc.h"
#include "sdcard.h"
#include "logger.h"
#include "led.h"
#include "uart.h"
#include "i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#define TAG "MAIN"
void app_main(void)
{
    	uart_init();
	uart_print_banner();
	i2c_init();
	led_init();
    	if (!sensor_init())
    {
        ESP_LOGI(TAG, "Sensor initialization failed");
        return;
    }

    ESP_LOGI(TAG, "Sensor initialized successfully");
if (!ds3231_init())
{
    ESP_LOGI(TAG, "RTC initialization failed");
    return;
}

ESP_LOGI(TAG, "RTC initialized successfully");    	
	if (!sdcard_init())
{
    ESP_LOGI(TAG, "SD card initialization failed");
    return;
}

ESP_LOGI(TAG, "SD card initialized successfully");
	if(!logger_init())
{
	ESP_LOGI(TAG, "Logger initialization failed");
	return;
}
xTaskCreate(
        logger_task,
        "logger_task",
        4096,
        NULL,
        5,
        NULL
    );

    ESP_LOGI(TAG, "Data logger task started");
	ESP_LOGI(TAG, "Logger initialized successfully");

    while (1)
    {
vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
