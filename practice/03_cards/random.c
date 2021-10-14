#include "random.h"

int random_get_num_in_range(int start, int end)
{
    return rand() % (end + 1 - start) + start;
}
