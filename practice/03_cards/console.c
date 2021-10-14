#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>
#include <stdbool.h>

#include "console.h"

void console_clear()
{
#if __unix__ || __linux__ || __APPLE__ || __MACH__
    system("clear");
#elif _WIN32
    system("cls");
#else
    puts("who are u?");
#endif
}

void console_str_to_lower(wchar_t* str)
{
    for (int i = 0; str[i]; i++) {
        str[i] = towlower(str[i]);
    }
}

int console_check_answer(wchar_t* str)
{
    console_str_to_lower(str);

    if (wcsncmp(str, L"y", 1) == 0 || wcsncmp(str, L"yes", 3) == 0) {
        return 1;
    } else if (wcsncmp(str, L"n", 1) == 0
        || wcsncmp(str, L"no", 2) == 0) {
        return 0;
    } else {
        return -1;
    }
}

bool console_yes_or_no(wchar_t* str)
{
    size_t buffer_size = 10;
    wchar_t buffer[buffer_size];
    wprintf(L"%ls [y/n] ", str);
    wscanf(L"%ls", buffer);
    int answer;
    while ((answer = console_check_answer(buffer)) == -1) {
        wprintf(L"%s [y/n] ", str);
        wscanf(L"%ls", buffer);
    }
    return answer;
}
