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
    Queue* queue = queue_new();
    queue_push_back(queue, card_new(CARD_SUIT_DIAMOND, CARD_VALUE_SIX));
    queue_push_back(queue, card_new(CARD_SUIT_DIAMOND, CARD_VALUE_QUEEN));
    queue_card_print(queue);
    queue_free(queue);
    return EXIT_SUCCESS;
}
