#ifndef LOGGER_HEADER_FILE_H
#define LOGGER_HEADER_FILE_H

#include <stdio.h>
#include <wchar.h>

void logger_print(FILE* file, wchar_t *message, ...);

#endif
