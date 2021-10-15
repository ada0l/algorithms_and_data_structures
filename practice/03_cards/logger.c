#include <stdio.h>
#include <time.h>

void logger_print(FILE* file, char* message)
{
    time_t t = time(NULL);
    struct tm* aTm = localtime(&t);
    fprintf(file, "%04d/%02d/%02d %02d:%02d:%02d: %s\n",
        aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday,
        aTm->tm_hour, aTm->tm_min, aTm->tm_sec, message);
}
