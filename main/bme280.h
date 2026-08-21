// bme280 header module
#ifndef BME280_H
#define BME280_H
#include <stdint.h>
#include <stdbool.h>
/*--------------------------------------------------
 * BME280 I2C Address
 *--------------------------------------------------*/
#define BME280_I2C_ADDRESS      0x76

/*--------------------------------------------------
 * Register Addresses
 *--------------------------------------------------*/
#define BME280_CHIP_ID_REG      0xD0
#define BME280_RESET_REGISTER        0xE0

/*--------------------------------------------------
 * Register Values
 *--------------------------------------------------*/
#define BME280_CHIP_ID          0x60
#define BME280_RESET_COMMAND    0xB6
#define BME280_CTRL_MEAS_REGISTER  0xF4
#define BME280_CTRL_HUM_REGISTER  0xF2
#define BME280_CONFIG_REGISTER  0xF5
#define BME280_TEMP_MSB_REGISTER    0xFA
#define BME280_CALIB_T1_REGISTER    0x88
#define BME280_PRESS_MSB_REGISTER    0xF7
#define BME280_CALIB_P1_REGISTER    0x8E
#define BME280_HUM_MSB_REGISTER    0xFD
#define BME280_CALIB_H1_REGISTER    0xA1
#define BME280_CALIB_H2_REGISTER    0xE1
#define BME280_CALIB_H3_REGISTER    0xE3
#define BME280_CALIB_H4_1_REGISTER    0xE4
#define BME280_CALIB_H5_REGISTER    0xE5
#define BME280_CALIB_H6_REGISTER    0xE7
typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
uint8_t dig_H1;
int16_t dig_H2;
uint8_t dig_H3;
int16_t dig_H4;
int16_t dig_H5;
int8_t dig_H6;
} bme280_calibration_t;
typedef struct {
float temperature;
    float pressure;
    float humidity;
} bme280_data_t;

bool bme280_init(void);
bool bme280_read_registers(uint8_t start_register,
                           uint8_t *buffer,
                           uint8_t length);
bool bme280_write_register(uint8_t reg, uint8_t value);
bool bme280_read_raw_temperature(uint32_t *raw_temperature);
bool bme280_read_calibration(void);
bool bme280_compensate_temperature(uint32_t adc_T, float *temperature);
bool bme280_read_temperature(float *temperature);
bool bme280_read_raw_pressure(uint32_t *raw_pressure);
bool bme280_read_pressure_calibration(void);
bool bme280_compensate_pressure(uint32_t adc_P, float *pressure);
bool bme280_read_pressure(float *pressure);
bool bme280_read_raw_humidity(uint16_t *raw_humidity);
bool bme280_read_humidity_calibration(void);
bool bme280_compensate_humidity(uint16_t adc_H, float *humidity);
bool bme280_read_data(bme280_data_t *data);
#endif
