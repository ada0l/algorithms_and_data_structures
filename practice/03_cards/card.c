#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "card.h"
#include "queue.h"

const int CARD_PRINT_ITERATIONS = 8;
const int CARD_PRINT_PATTERN_OFFSET = -2;
const wchar_t* CARD_PRINT_PATTERN[11][5] = {
    {
        // 0 - not_used
    },
    {
        // 1 - not_used
    },
    {
        // 2
        L"│         │",
        L"│    %lc    │",
        L"│         │",
        L"│    %lc    │",
        L"│         │",
    },
    {
        // 3
        L"│    %lc    │",
        L"│         │",
        L"│    %lc    │",
        L"│         │",
        L"│    %lc    │",
    },
    {
        // 4
        L"│  %lc   %lc  │",
        L"│         │",
        L"│         │",
        L"│         │",
        L"│  %lc   %lc  │",
    },
    {
        // 5
        L"│  %lc   %lc  │",
        L"│         │",
        L"│    %lc    │",
        L"│         │",
        L"│  %lc   %lc  │",
    },
    {
        // 6
        L"│  %lc   %lc  │",
        L"│         │",
        L"│  %lc   %lc  │",
        L"│         │",
        L"│  %lc   %lc  │",
    },
    {
        // 7
        L"│  %lc   %lc  │",
        L"│    %lc    │",
        L"│  %lc   %lc  │",
        L"│         │",
        L"│  %lc   %lc  │",
    },
    {
        // 8
        L"│  %lc   %lc  │",
        L"│    %lc    │",
        L"│  %lc   %lc  │",
        L"│    %lc    │",
        L"│  %lc   %lc  │",
    },
    {
        // 9
        L"│  %lc   %lc  │",
        L"│    %lc    │",
        L"│  %lc %lc %lc  │",
        L"│    %lc    │",
        L"│  %lc   %lc  │",
    },
    {
        // 10
        L"│  %lc   %lc  │",
        L"│  %lc   %lc  │",
        L"│  %lc   %lc  │",
        L"│  %lc   %lc  │",
        L"│  %lc   %lc  │",
    },
};

Card* card_new(int suit, int value)
{
    Card* card = (Card*)malloc(sizeof(Card));
    card->suit = suit;
    card->value = value;
    return card;
}

void card_free(Card* card) { free(card); }

wchar_t card_get_char_of_value(Card* card)
{
    wchar_t c = 0;
    switch (card->value) {
    case CARD_VALUE_QUEEN:
        c = 0x1f339;
        break;
    case CARD_VALUE_KING:
        c = 0x1f451;
        break;
    case CARD_VALUE_JOKER:
        c = 0x1f97e;
        break;
    case CARD_VALUE_ACE:
        c = 0x1f1e6;
        break;
    }
    return c;
}

wchar_t card_get_char_of_suit(Card* card)
{
    wchar_t c = 0;
    switch (card->suit) {
    case CARD_SUIT_DIAMOND:
        c = 0x2666;
        break;
    case CARD_SUIT_HEARTH:
        c = 0x2665;
        break;
    case CARD_SUIT_SPADES:
        c = 0x2660;
        break;
    case CARD_SUIT_CLUBS:
        c = 0x2663;
        break;
    }
    return c;
}
wchar_t* card_to_wchar_t_ptr(Card* card)
{
    wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t) * 4);
    buffer[0] = card_get_char_of_suit(card);
    wchar_t value = card_get_char_of_value(card);
    if (value != 0) {
        buffer[1] = value;
        buffer[2] = '\0';
    } else {
        buffer[1] = (card->value / 10 != 0) ? '0' + (card->value / 10)
                                            : ' ';
        buffer[2] = '0' + (card->value % 10);
        buffer[3] = '\0';
    }
    return buffer;
}

bool card_is_number(Card* card)
{
    return CARD_VALUE_TWO <= card->value
        && card->value <= CARD_VALUE_TEN;
}

void card_print(FILE* file, Card* card, int iteration)
{
    wchar_t c_value = card_get_char_of_value(card);
    wchar_t c_suit = card_get_char_of_suit(card);

    if (iteration == 0) {
        fwprintf(file, L"┌─────────┐");
    } else if (iteration == 1 && c_value == 0) {
        fwprintf(file, L"│%2d       │", card->value);
    } else if (iteration == 1 && c_value != 0) {
        fwprintf(file, L"│%3lc     │", c_value);
    } else if (2 <= iteration && iteration <= 6
        && card_is_number(card)) {
        fwprintf(file,
            CARD_PRINT_PATTERN[card->value]
                              [iteration + CARD_PRINT_PATTERN_OFFSET],
            c_suit, c_suit, c_suit);
    } else if (iteration == 4 && !card_is_number(card)) {
        fwprintf(file, L"│    %lc    │", c_suit);
    } else if (iteration == 7 && c_value == 0) {
        fwprintf(file, L"│      %2d │", card->value);
    } else if (iteration == 7 && c_value != 0) {
        fwprintf(file, L"│    %3lc │", c_value);
    } else if (iteration == 8) {
        fwprintf(file, L"└─────────┘");
    } else {
        fwprintf(file, L"│         │");
    }
}

void card_print_simple(FILE* file, Card* card)
{
    wchar_t c_value = card_get_char_of_value(card);
    wchar_t c_suit = card_get_char_of_suit(card);
    if (c_value) {
        fwprintf(file, L"%lc%lc", c_suit, c_value);
    } else {
        fwprintf(file, L"%lc%d", c_suit, card->value);
    }
}

Queue* card_new_queue()
{
    Queue* queue = queue_new();
    for (int value = CARD_VALUE_ACE; value <= CARD_VALUE_JOKER;
         ++value) {
        for (int suit = CARD_SUIT_DIAMOND; suit <= CARD_SUIT_CLUBS;
             ++suit) {
            queue_push_back(queue, card_new(suit, value));
        }
    }
    return queue;
}

void queue_card_print(FILE* file, Queue* queue)
{
    for (int i = 0; i <= CARD_PRINT_ITERATIONS; ++i) {
        for (QueueNode* node = queue->beg->next;
             queue_is_node_dereferenable(queue, node);
             node = node->next) {
            card_print(file, node->data, i);
        }
        fwprintf(file, L"\n");
    }
}

void queue_card_print_simple(FILE* file, Queue* queue)
{
    for (QueueNode* node = queue->beg->next; node != queue->end;
         node = node->next) {
        card_print_simple(file, node->data);
        fwprintf(file, L" ");
    }
}
