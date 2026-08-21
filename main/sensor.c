// Sensor C-Module
#include "sensor.h"

#include "esp_log.h"
#include "bme280.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
static const char *TAG = "SENSOR";
static void sensor_task(void *pvParameters);
bool sensor_init(void)
{
    ESP_LOGI(TAG, "Sensor Module Initialized");
if(xTaskCreate(
    sensor_task,
    "sensor_task",
    4096,
    NULL,
    5,
    NULL
)!= pdPASS)
{
    printf("Failed to create sensor task\n");
    return false;
}
	return bme280_init();
}

bool sensor_read(bme280_data_t *data)
{
    if (data == NULL)
    {
        return false;
    }
	return bme280_read_data(data);
}
static void sensor_task(void *pvParameters)
{
    while (1)
    {
        bme280_data_t data;
        if (sensor_read(&data))
        {
            printf("Temperature: %.2f C\n", data.temperature);
            printf("Pressure: %.2f Pa\n", data.pressure);
            printf("Humidity: %.2f %%\n", data.humidity);
        }
        else
        {
            printf("BME280 read failed\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
