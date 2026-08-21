# ESP32 Environmental Monitoring Data Logger

## Project Overview

An ESP32-based environmental monitoring data logger developed using ESP-IDF and C.

The system is designed to collect environmental parameters from a BME280 sensor, obtain date and time information from a DS3231 RTC, and store the measurements on a microSD card in CSV format.

The firmware is organized into independent modules for sensor handling, RTC, SD card, logging, UART and LED control.

---

## System Architecture

```text
                 ┌──────────────┐
                 │    BME280    │
                 │ Temperature  │
                 │ Humidity     │
                 │ Pressure     │
                 └──────┬───────┘
                        │ I2C
                        │
                 ┌──────▼───────┐
                 │    ESP32     │
                 │              │
                 │  FreeRTOS    │
                 │    Logger    │
                 └───┬──────┬───┘
                     │      │
                  I2C│      │SPI
                     │      │
             ┌───────▼──┐ ┌─▼────────┐
             │  DS3231  │ │ MicroSD  │
             │   RTC    │ │  Card    │
             └──────────┘ └────┬─────┘
                               │
                               ▼
                           data.csv
Hardware
ESP32
BME280 Environmental Sensor
DS3231 Real-Time Clock
MicroSD Card
LED
USB/UART interface

Software
Embedded C
ESP-IDF
FreeRTOS
I2C
SPI
FAT filesystem
Git/GitHub
KiCad

Firmware Modules
main/
├── app_main.c
├── bme280.c
├── bme280.h
├── sensor.c
├── sensor.h
├── rtc.c
├── rtc.h
├── sdcard.c
├── sdcard.h
├── logger.c
├── logger.h
├── led.c
├── led.h
├── uart.c
└── uart.h

Module Description
BME280 – Interfaces with the environmental sensor.
Sensor – Provides a simple interface for sensor initialization and readings.
RTC – Interfaces with the DS3231 real-time clock.
SD Card – Handles microSD card initialization and file operations.
Logger – Combines sensor and RTC data and stores it on the SD card.
LED – Provides status indication.
UART – Provides serial communication/debugging.

current Status
 ESP-IDF project setup
 UART module
 BME280 module
 Sensor module
 DS3231 RTC module
 SD card SPI module
 CSV file creation
 CSV data append
 FreeRTOS logging task
 Prevent CSV overwrite on restart
 Hardware testing
 PCB design
 PCB manufacturing
 Long-duration logging test

Future Improvements
Hardware validation
Finalize logging interval
SD card error handling
Custom PCB using KiCad
Long-duration environmental data logging
Data analysis using the generated CSV files
Enclosure design

Project Goal

This project demonstrates practical skills in:

Embedded C programming
ESP32 and ESP-IDF development
FreeRTOS
I2C and SPI communication
Sensor interfacing
RTC interfacing
SD card data logging
Modular embedded firmware development
PCB design using KiCad
