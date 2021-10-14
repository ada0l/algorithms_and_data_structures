#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "card.h"
#include "queue.h"

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.utf8");
    Queue* queue = card_new_queue();
    wprintf(L"size: %d\n", queue->size);

    queue_card_print(queue);
    queue_shuffle(queue);
    wprintf(L"\n");
    queue_card_print(queue);
    queue_free(queue);

    return EXIT_SUCCESS;
}
