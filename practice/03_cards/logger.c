#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>

void logger_print(FILE* file, wchar_t *message, ...)
{
    time_t t = time(NULL);
    struct tm* aTm = localtime(&t);
    fwprintf(file, L"%04d/%02d/%02d %02d:%02d:%02d: ",
        aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
        aTm->tm_hour, aTm->tm_min, aTm->tm_sec);

    va_list ap;
    wchar_t *str = message;

    va_start(ap, message);
    while (str != NULL) {
        fwprintf(file, L"%ls ", str);
        str = va_arg(ap, wchar_t*);
    }
    va_end(ap);

    fwprintf(file, L"\n");
}
