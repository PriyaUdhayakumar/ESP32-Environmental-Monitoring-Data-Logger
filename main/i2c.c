//i2c c module
#include "i2c.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

#define I2C_SCL_PIN      GPIO_NUM_22
#define I2C_SDA_PIN      GPIO_NUM_21

#define I2C_CLOCK_HZ     100000
static i2c_master_bus_handle_t bus_handle = NULL;
static const char *TAG = "I2C";

bool i2c_init(void)
{
	i2c_master_bus_config_t bus_config =
{
    .i2c_port = I2C_NUM_0,

    .sda_io_num = I2C_SDA_PIN,

    .scl_io_num = I2C_SCL_PIN,

    .clk_source = I2C_CLK_SRC_DEFAULT,

    .glitch_ignore_cnt = 7,

    .flags.enable_internal_pullup = true,
};
esp_err_t ret;

    ret = i2c_new_master_bus(
                &bus_config,
                &bus_handle);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to create I2C bus");
        return false;
    }


    ESP_LOGI(TAG, "I2C Initialized");

    return true;
}
 i2c_master_bus_handle_t i2c_get_bus_handle(void)
{
    return bus_handle;
}
bool i2c_write(uint8_t device_address,
               uint8_t register_address,
               const uint8_t *data,
               uint8_t length)
{
    return true;
}

bool i2c_read(uint8_t device_address,
              uint8_t register_address,
              uint8_t *data,
              uint8_t length)
{
    return true;
}
