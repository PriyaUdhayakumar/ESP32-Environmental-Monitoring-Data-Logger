// rtc c module
#include "rtc.h"
#include "i2c.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static uint8_t bcd_to_decimal(uint8_t value);
static uint8_t decimal_to_bcd(uint8_t value);
static i2c_master_dev_handle_t device_handle = NULL;
static const char *TAG = "RTC";
static bool ds3231_read_registers(
    uint8_t register_address,
    uint8_t *data,
    uint8_t length);
static bool ds3231_write_register(
    uint8_t register_address,
    const uint8_t *data,
    uint8_t length);
bool ds3231_init(void)
{
i2c_device_config_t dev_config =
{
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,

    .device_address = DS3231_I2C_ADDRESS ,

    .scl_speed_hz = I2C_CLOCK_HZ,
};
esp_err_t ret;

ret = i2c_master_bus_add_device(
            i2c_get_bus_handle(),
            &dev_config,
            &device_handle);
if (ret != ESP_OK)
{
    ESP_LOGE(TAG, "Failed to add RTC");
    return false;
}
	return true;
}
static uint8_t bcd_to_decimal(uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}
static uint8_t decimal_to_bcd(uint8_t value)
{
    return ((value / 10) << 4) | (value % 10);
}
static bool ds3231_read_registers(
    uint8_t register_address,
    uint8_t *data,
    uint8_t length)
{
    return i2c_read(
        DS3231_I2C_ADDRESS,
        register_address,
        data,
        length);
}
static bool ds3231_write_register(
    uint8_t register_address,
    const uint8_t *data,
    uint8_t length)
{
    return i2c_write(
        DS3231_I2C_ADDRESS,
        register_address,
        data,
        length);
}
bool rtc_get_time(rtc_time_t *time)
{
    uint8_t data[7];

    if (time == NULL)
    {
        return false;
    }

    if (!ds3231_read_registers(
        DS3231_SECONDS_REGISTER,
        data,
        7))
    {
        return false;
    }

    time->second = bcd_to_decimal(data[0] & 0x7F);
time->minute = bcd_to_decimal(data[1] & 0x7F);

if (data[2] & 0x40)
{
    uint8_t hour = bcd_to_decimal(data[2] & 0x1F);

    if (data[2] & 0x20)
    {
        time->hour = (hour == 12) ? 12 : hour + 12;
    }
    else
    {
        time->hour = (hour == 12) ? 0 : hour;
    }
}
else
{
    time->hour = bcd_to_decimal(data[2] & 0x3F);
}

time->day   = bcd_to_decimal(data[4] & 0x3F);
time->month = bcd_to_decimal(data[5] & 0x1F);
time->year  = 2000 + bcd_to_decimal(data[6]);

    return true;

}
bool rtc_set_time(const rtc_time_t *time)
{
    uint8_t data[7];

    if (time == NULL)
    {
        return false;
    }
if (time->year < 2000 || time->year > 2099)
    {
        return false;
    }

    if (time->month < 1 || time->month > 12)
    {
        return false;
    }

    if (time->day < 1 || time->day > 31)
    {
        return false;
    }

    if (time->hour > 23)
    {
        return false;
    }

    if (time->minute > 59)
    {
        return false;
    }

    if (time->second > 59)
 {
        return false;
    }

    data[0] = decimal_to_bcd(time->second);
    data[1] = decimal_to_bcd(time->minute);
    data[2] = decimal_to_bcd(time->hour);
    data[3] = 1;
    data[4] = decimal_to_bcd(time->day);
    data[5] = decimal_to_bcd(time->month);
    data[6] = decimal_to_bcd(time->year - 2000);

    return ds3231_write_register(
        DS3231_SECONDS_REGISTER,
        data,
        7);
}
