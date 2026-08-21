//sdcard c module
#include "sdcard.h"
#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"

#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"
#define SD_MOSI_PIN  GPIO_NUM_23
#define SD_MISO_PIN  GPIO_NUM_19
#define SD_SCLK_PIN  GPIO_NUM_18
#define SD_CS_PIN    GPIO_NUM_5

#define SD_MOUNT_POINT "/sdcard"

static const char *TAG = "SDCARD";

static sdmmc_card_t *card = NULL;
bool sdcard_init(void)
{
    esp_err_t ret;

    spi_bus_config_t bus_cfg =
    {
        .mosi_io_num = SD_MOSI_PIN,
        .miso_io_num = SD_MISO_PIN,
        .sclk_io_num = SD_SCLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(
        SPI2_HOST,
        &bus_cfg,
        SDSPI_DEFAULT_DMA);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPI bus");
        return false;
    }

    sdspi_device_config_t slot_config =
        SDSPI_DEVICE_CONFIG_DEFAULT();

    slot_config.gpio_cs = SD_CS_PIN;
    slot_config.host_id = SPI2_HOST;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    esp_vfs_fat_mount_config_t mount_config =
    {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };

    ret = esp_vfs_fat_sdspi_mount(
        SD_MOUNT_POINT,
        &host,
        &slot_config,
        &mount_config,
        &card);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount SD card: %s",
                 esp_err_to_name(ret));

        spi_bus_free(SPI2_HOST);

        return false;
    }

    ESP_LOGI(TAG, "SD card initialized successfully");

    return true;
}
bool sdcard_write_file(const char *filename, const char *data)
{
    char path[128];

    snprintf(path, sizeof(path), "%s/%s",
             SD_MOUNT_POINT, filename);

    ESP_LOGI(TAG, "Writing file: %s", path);

    FILE *file = fopen(path, "w");

    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return false;
    }

    fprintf(file, "%s", data);

    fclose(file);

    ESP_LOGI(TAG, "File written successfully");

    return true;
}
bool sdcard_append_file(const char *filename, const char *data)
{
    char path[128];

    snprintf(path, sizeof(path), "%s/%s",
             SD_MOUNT_POINT, filename);

    FILE *file = fopen(path, "a");

    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for appending");
        return false;
    }

    fprintf(file, "%s", data);

    fclose(file);

    return true;
}
bool sdcard_read_file(const char *filename)
{
    char path[128];

    snprintf(path, sizeof(path), "%s/%s",
             SD_MOUNT_POINT, filename);

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return false;
    }

    char buffer[128];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);

    return true;
}
bool sdcard_file_exists(const char *filename)
{
    char path[128];

    snprintf(path, sizeof(path), "%s/%s",
             SD_MOUNT_POINT, filename);

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        return false;
    }

    fclose(file);

    return true;
}
