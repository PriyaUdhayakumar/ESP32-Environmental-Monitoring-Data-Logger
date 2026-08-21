// Sensor module
#ifndef SENSOR_H
#define SENSOR_H
//#include "environment.h"
#include <stdbool.h>
#include "bme280.h"


bool sensor_init(void);

bool sensor_read(bme280_data_t *data);
#endif
