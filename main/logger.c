//logger c file
#include "logger.h"
#include <stdio.h>

#include "esp_log.h"
//#include"environment.h"
#include "sensor.h"
#include "rtc.h"
#include "sdcard.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define TAG "LOGGER"
#define LOG_INTERVAL_MS 5000
#define LOG_FILE "data.csv"
bool logger_init(void)
{
    /*
     * Create the CSV file and write the header
     */

    const char *header = "Date,Time,Temperature,Humidity,Pressure\n";
 if (sdcard_file_exists(LOG_FILE))
    {
        ESP_LOGI(TAG, "Log file already exists");

        return true;
    }

    ESP_LOGI(TAG, "Creating new log file");        

    if (!sdcard_write_file(LOG_FILE, header))
    {
        ESP_LOGE(TAG, "Failed to create log file");

        return false;
    }

    ESP_LOGI(TAG, "Log file initialized");

    return true;
}
bool logger_log_data(void)
{
    bme280_data_t sensor_data;
    rtc_time_t time;

    char log_line[200];

    /*
     * Read sensor
     */

    if (!sensor_read(&sensor_data))
    {
        ESP_LOGE(TAG, "Failed to read sensor");

        return false;
    }

    /*
     * Read RTC
     */
if (!rtc_get_time(&time))
    {
        ESP_LOGE(TAG, "Failed to read RTC");

        return false;
    }

    /*
     * Create CSV line
     */
snprintf(
        log_line,
        sizeof(log_line),
        "%04d-%02d-%02d,%02d:%02d:%02d,%.2f,%.2f,%.2f\n",
        time.year,
        time.month,
        time.day,
        time.hour,
        time.minute,
        time.second,
        sensor_data.temperature,
        sensor_data.humidity,
        sensor_data.pressure
    );
/*
     * Append data to SD card
     */

    if (!sdcard_append_file(LOG_FILE, log_line))
    {
        ESP_LOGE(TAG, "Failed to write sensor data");

        return false;
    }

    ESP_LOGI(TAG, "Data logged: %s", log_line);

    return true;
}
void logger_task(void *pvParameters)
{
    while (1)
    {
        if (!logger_log_data())
        {
            ESP_LOGE(TAG, "Failed to log environmental data");
        }

        vTaskDelay(pdMS_TO_TICKS( LOG_INTERVAL_MS));
    }
}
