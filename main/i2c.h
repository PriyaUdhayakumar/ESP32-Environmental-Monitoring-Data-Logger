// i2c header file
#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/i2c_master.h"
#define I2C_CLOCK_HZ 100000
#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_PIN     GPIO_NUM_21
#define I2C_SCL_PIN     GPIO_NUM_22
bool i2c_init(void);
 i2c_master_bus_handle_t i2c_get_bus_handle(void);

bool i2c_write(uint8_t device_address,
               uint8_t register_address,
               const uint8_t *data,
               uint8_t length);

bool i2c_read(uint8_t device_address,
              uint8_t register_address,
              uint8_t *data,
              uint8_t length);

#endif
