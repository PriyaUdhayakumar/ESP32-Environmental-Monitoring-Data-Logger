#include "led.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2

static const char *TAG = "LED";

void led_init(void)
{
    gpio_config_t led_config =
    {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&led_config);

    gpio_set_level(LED_GPIO, 0);

    ESP_LOGI(TAG, "LED Initialized");
}

void led_on(void)
{
    gpio_set_level(LED_GPIO, 1);
}

void led_off(void)
{
    gpio_set_level(LED_GPIO, 0);
}

void led_toggle(void)
{
    static bool led_state = false;

    led_state = !led_state;

    gpio_set_level(LED_GPIO, led_state);
}
