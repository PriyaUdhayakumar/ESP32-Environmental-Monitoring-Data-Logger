//uart c file
#include "uart.h"
#include "esp_log.h"
static const char *TAG = "UART";
void uart_init(void)
{
	ESP_LOGI(TAG, "UART Logger Initialized");
}
void uart_print_banner(void)
{ 	ESP_LOGI(TAG, "=================================");
    	ESP_LOGI(TAG, " Project10 - ESP32 Data Logger");
    	ESP_LOGI(TAG, " ESP-IDF Firmware Started");
    	ESP_LOGI(TAG, "=================================");
}
