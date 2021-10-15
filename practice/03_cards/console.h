#ifndef CONSOLE_HEADER_FILE_H
#define CONSOLE_HEADER_FILE_H

#include <wchar.h>
#include <stdbool.h>

void console_sleep(int time);

void console_clear();

void console_str_to_lower(wchar_t* str);

int console_check_answer(wchar_t* str);

bool console_yes_or_no(wchar_t* str);

#endif
