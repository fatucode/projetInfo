
#ifndef CSV_READER_H
#define CSV_READER_H
#include <stdio.h>

typedef void (*csv_line_cb)(const char* col1, const char* amont, const char* aval,
                            const char* volume, const char* fuite, void* user_data);
int csv_process_file(const char* filename, csv_line_cb cb, void* user_data);

#endif
