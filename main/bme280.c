//bme280 c module
#include "bme280.h"
#include "i2c.h"
#include<stdint.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static i2c_master_dev_handle_t device_handle = NULL;
static const char *TAG = "BME280";
static bme280_calibration_t calibration;
static int32_t t_fine;
bool bme280_read_registers(uint8_t start_register,
                           uint8_t *buffer,
                           uint8_t length)
{
    esp_err_t ret;

   ret = i2c_master_transmit_receive(
                device_handle,&start_register,
                1,
                buffer,
                length,
                -1);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read %d bytes from register 0x%02X",
                 length,
                 start_register);
        return false;
    }

    return true;
}
bool bme280_init(void)
{
i2c_device_config_t dev_config =
{
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,

    .device_address = BME280_I2C_ADDRESS,

    .scl_speed_hz = I2C_CLOCK_HZ,
};
esp_err_t ret;

ret = i2c_master_bus_add_device(
            i2c_get_bus_handle(),
            &dev_config,
            &device_handle);
if (ret != ESP_OK)
{
    ESP_LOGE(TAG, "Failed to add BME280");

    return false;
}
if (!bme280_write_register(
        BME280_RESET_REGISTER,
        BME280_RESET_COMMAND))
{
    return false;
}
vTaskDelay(pdMS_TO_TICKS(2));
uint8_t chip_id;

if (!bme280_read_registers(
        BME280_CHIP_ID_REG,
        &chip_id,1))
{
    return false;
}

ESP_LOGI(TAG, "BME280 detected successfully");

return true;
if (!bme280_read_calibration())
{
    return false;
}
if (!bme280_read_pressure_calibration())
{
    return false;
}
if (!bme280_read_humidity_calibration())
{
    return false;
}
uint8_t ctrl_hum = 0;

ctrl_hum |= (1 << 0);
if (!bme280_write_register(
        BME280_CTRL_HUM_REGISTER,
        ctrl_hum))
{
    return false;
}
uint8_t config = 0;

config |= (5 << 5);

if (!bme280_write_register(
        BME280_CONFIG_REGISTER,
        config))
{
    return false;
}
uint8_t ctrl_meas = 0;

ctrl_meas |= (1 << 5);
ctrl_meas |= (1 << 2);
ctrl_meas |= (3 << 0);
if (!bme280_write_register(
        BME280_CTRL_MEAS_REGISTER,
        ctrl_meas))
{
    return false;
}
}
bool bme280_write_register(uint8_t reg, uint8_t value)
{
    uint8_t buffer[2];

    buffer[0] = reg;
    buffer[1] = value;

    esp_err_t ret = i2c_master_transmit(
                        device_handle,
                        buffer,
                        2,
                        -1);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "Failed to write register 0x%02X",
                 reg);

        return false;
    }

    return true;
}
bool bme280_read_raw_temperature(uint32_t *raw_temperature)
{
    uint8_t data[3];

    if (!bme280_read_registers(
            BME280_TEMP_MSB_REGISTER,
            data,
            3))
    {
        return false;
    }

    *raw_temperature =
        ((uint32_t)data[0] << 12) |
        ((uint32_t)data[1] << 4)  |
        ((uint32_t)data[2] >> 4);

    return true;
}
bool bme280_read_calibration(void)
{
    uint8_t data[6];

    if (!bme280_read_registers(
            BME280_CALIB_T1_REGISTER,
            data,
            6))
    {
        return false;
    }

    calibration.dig_T1 =
        (uint16_t)data[0] |
        ((uint16_t)data[1] << 8);

    calibration.dig_T2 =
        (int16_t)(
            (uint16_t)data[2] |
            ((uint16_t)data[3] << 8)
        );

    calibration.dig_T3 =
        (int16_t)(
            (uint16_t)data[4] |
            ((uint16_t)data[5] << 8)
        );

    return true;
}
bool bme280_compensate_temperature(uint32_t adc_T, float *temperature)
{
    int32_t var1;
    int32_t var2;
 int32_t temperature_int;
 var1 = ((((int32_t)adc_T >> 3) -
             ((int32_t)calibration.dig_T1 << 1)) *
             ((int32_t)calibration.dig_T2)) >> 11;
var2 = (((((int32_t)adc_T >> 4) -
              ((int32_t)calibration.dig_T1)) *
             (((int32_t)adc_T >> 4) -
              ((int32_t)calibration.dig_T1)) >> 12) *
             ((int32_t)calibration.dig_T3)) >> 14;

    t_fine = var1 + var2;
     temperature_int = (t_fine * 5 + 128) >> 8;

    *temperature = temperature_int / 100.0f;    
return true;
}
bool bme280_read_temperature(float *temperature)
{
    uint32_t adc_T;

    if (!bme280_read_raw_temperature(&adc_T))
    {
        return false;
    }

    if (!bme280_compensate_temperature(adc_T, temperature))
    {
        return false;
    }

    return true;
}

bool bme280_read_raw_pressure(uint32_t *raw_pressure)
{
    uint8_t data[3];

    if (!bme280_read_registers(
            BME280_PRESS_MSB_REGISTER,
            data,
            3))
    {
        return false;
    }

    *raw_pressure =
        ((uint32_t)data[0] << 12) |
        ((uint32_t)data[1] << 4)  |
        ((uint32_t)data[2] >> 4);

    return true;
}
bool bme280_read_pressure_calibration(void)
{
    uint8_t data[18];

    if (!bme280_read_registers(
            BME280_CALIB_P1_REGISTER,
            data,
            18))
    {
        return false;
    }
calibration.dig_P1 =
    (uint16_t)data[0] |
    ((uint16_t)data[1] << 8);
calibration.dig_P2 =
    (int16_t)(
        (uint16_t)data[2] |
        ((uint16_t)data[3] << 8)
    );
calibration.dig_P3 =
    (int16_t)(
        (uint16_t)data[4] |
        ((uint16_t)data[5] << 8)
    );

calibration.dig_P4 =
    (int16_t)(
        (uint16_t)data[6] |
        ((uint16_t)data[7] << 8)
    );

calibration.dig_P5 =
    (int16_t)(
        (uint16_t)data[8] |
        ((uint16_t)data[9] << 8)
    );

calibration.dig_P6 =
    (int16_t)(
        (uint16_t)data[10] |
        ((uint16_t)data[11] << 8)
    );

calibration.dig_P7 =
    (int16_t)(
        (uint16_t)data[12] |
        ((uint16_t)data[13] << 8)
    );

calibration.dig_P8 =
    (int16_t)(
        (uint16_t)data[14] |
        ((uint16_t)data[15] << 8)
    );

calibration.dig_P9 =
    (int16_t)(
        (uint16_t)data[16] |
        ((uint16_t)data[17] << 8)
    );
    return true;
}
bool bme280_compensate_pressure(uint32_t adc_P, float *pressure)
{
    int64_t var1;
    int64_t var2;
    int64_t p;
var1 = ((int64_t)t_fine) - 128000;
var2 = var1 * var1 *
           (int64_t)calibration.dig_P6;
var2 = var2 +
           ((var1 * (int64_t)calibration.dig_P5) << 17);
var2 = var2 +
           ((int64_t)calibration.dig_P4 << 35);
var1 = ((var1 * var1 *
             (int64_t)calibration.dig_P3) >> 8) +
           ((var1 * (int64_t)calibration.dig_P2) << 12);
var1 = (((int64_t)1 << 47) + var1) *
       (int64_t)calibration.dig_P1 >> 33;
if (var1 == 0)
{
    return false;
}

p = ((int64_t)adc_P << 31) - var1;
p = (p * 3125) / var1;

p = p + ((p >> 13) * (p >> 13) *
         (int64_t)calibration.dig_P9 >> 25);

p = p + (((p >> 19) *
          (int64_t)calibration.dig_P8) >> 16);
p = p + ((int64_t)calibration.dig_P7 << 4);
p = p >> 8;

*pressure = (float)p;
    return true;
}
bool bme280_read_pressure(float *pressure)
{
    uint32_t adc_P;

    if (pressure == NULL)
    {
        return false;
    }

    if (!bme280_read_raw_pressure(&adc_P))
    {
        return false;
    }

    if (!bme280_compensate_pressure(adc_P, pressure))
    {
        return false;
    }

    return true;
}
bool bme280_read_raw_humidity(uint16_t *raw_humidity)
{
    uint8_t data[2];

    if (raw_humidity == NULL)
    {
        return false;
    }

    if (!bme280_read_registers(
            BME280_HUM_MSB_REGISTER,
            data,
            2))
    {
        return false;
    }

    *raw_humidity =
        ((uint16_t)data[0] << 8) |
        (uint16_t)data[1];

    return true;
}
bool bme280_read_humidity_calibration(void)
{
uint8_t data[2];
    if (!bme280_read_registers(
            BME280_CALIB_H1_REGISTER,
            data,
            1))
    {
        return false;
    }

    calibration.dig_H1 = data[0];
/* dig_H2 */
    if (!bme280_read_registers(
            BME280_CALIB_H2_REGISTER,
            data,
            2))
    {
        return false;
    }

    calibration.dig_H2 =
        (int16_t)(
            (uint16_t)data[0] |
            ((uint16_t)data[1] << 8)
        );

    /* dig_H3 */
    if (!bme280_read_registers(
            BME280_CALIB_H3_REGISTER,
            data,
            1))
    {
        return false;
    }

    calibration.dig_H3 = data[0];
/*dig-H4*/
if (!bme280_read_registers(
            BME280_CALIB_H4_1_REGISTER,
            data,
            1))
    {
        return false;
    }
 calibration.dig_H4 =(int16_t)(((uint16_t)data[0]<<4)|((uint16_t)data[1]&0x0F));
if (!bme280_read_registers(
            BME280_CALIB_H5_REGISTER,
            data,
            1))
    {
        return false;
    }
 calibration.dig_H4 =(int16_t)(((uint16_t)data[0]<<4)|((uint16_t)data[1]>>4));
 if (!bme280_read_registers(
            BME280_CALIB_H3_REGISTER,
            data,
            1))
    {
        return false;
    }

    calibration.dig_H6 = (int8_t)data[0];
return true;
}
bool bme280_compensate_humidity(uint16_t adc_H, float *humidity)
{
    int32_t v_x1_u32r;

    if (humidity == NULL)
    {
        return false;
    }
v_x1_u32r = t_fine - 76800;
v_x1_u32r =
    v_x1_u32r -
    (((((int32_t)adc_H << 14) -
       ((int32_t)calibration.dig_H4 << 20) -
       ((int32_t)calibration.dig_H5 * v_x1_u32r)) +
      16384) >> 15) *
    (((((((v_x1_u32r * (int32_t)calibration.dig_H6) >> 10) *
         (((v_x1_u32r * (int32_t)calibration.dig_H3) >> 11) +
          32768)) >> 10) +
       2097152) *
      (int32_t)calibration.dig_H2 + 8192) >> 14);
v_x1_u32r =
        v_x1_u32r -
        (((((v_x1_u32r >> 15) *
            (v_x1_u32r >> 15)) >> 7) *
          (int32_t)calibration.dig_H1) >> 4);

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;

    *humidity = (float)(v_x1_u32r >> 12) / 1024.0f;
    return true;
}
bool bme280_read_data(bme280_data_t *data)
{
    if (data == NULL)
    {
        return false;
    }
uint8_t raw_data[8];

if (!bme280_read_registers(0xF7, raw_data, 8))
{
    return false;
}
int32_t adc_T;

adc_T = ((int32_t)raw_data[3] << 12) |
        ((int32_t)raw_data[4] << 4) | ((int32_t)raw_data[5] >> 4);
int32_t adc_P;

adc_P = ((int32_t)raw_data[0] << 12) |
        ((int32_t)raw_data[1] << 4) |
        ((int32_t)raw_data[2] >> 4);
int32_t adc_H;
adc_H= ((int32_t)raw_data[6]<<8)|((int32_t)raw_data[7]);
if (!bme280_compensate_temperature(adc_T, &data->temperature))
{
    return false;
}
if (!bme280_compensate_pressure(adc_P, &data->pressure))
{
    return false;
}
if (!bme280_compensate_humidity(adc_H, &data->humidity))
{
    return false;
}
    return true;
}
