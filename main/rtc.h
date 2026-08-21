// rtc header module
#ifndef RTC_H
#define RTC_H
#include <stdint.h>
#include <stdbool.h>
#define DS3231_I2C_ADDRESS       0x68
#define DS3231_SECONDS_REGISTER  0x00
#define DS3231_MINUTES_REGISTER  0x01
#define DS3231_HOURS_REGISTER    0x02
#define DS3231_DAY_REGISTER      0x03
#define DS3231_DATE_REGISTER     0x04
#define DS3231_MONTH_REGISTER    0x05
#define DS3231_YEAR_REGISTER     0x06
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_time_t;
bool ds3231_init(void);
bool rtc_get_time(rtc_time_t *time);
bool rtc_set_time(const rtc_time_t *time);
#endif

