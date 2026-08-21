//sdcard header module
#ifndef SDCARD_H
#define SDCARD_H

bool sdcard_init(void);
bool sdcard_write_file(
    const char *filename,
    const char *data);

bool sdcard_append_file(
    const char *filename,
    const char *data);
bool sdcard_read_file(const char *filename);
bool sdcard_file_exists(const char *filename);
#endif
