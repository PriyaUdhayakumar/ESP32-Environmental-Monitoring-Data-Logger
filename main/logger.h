//logger header file
#ifndef LOGGER_H
#define LOGGER_H
#include <stdbool.h>
bool logger_init(void);
bool logger_log_data(void);
void logger_task(void *pvParameters);

#endif
