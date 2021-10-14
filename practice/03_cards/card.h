#ifndef CARD_HEADER_FILE_H
#define CARD_HEADER_FILE_H

#include "queue.h"

extern const int CARD_PRINT_ITERATIONS;
extern const int CARD_PRINT_PATTERN_OFFSET;
extern const wchar_t* CARD_PRINT_PATTERN[11][5];

enum CARD_SUIT {
    CARD_SUIT_DIAMOND = 1,
    CARD_SUIT_HEARTH = 2,
    CARD_SUIT_SPADES = 3,
    CARD_SUIT_CLUBS = 4
};

enum CARD_VALUE {
    CARD_VALUE_ACE = 1,
    CARD_VALUE_TWO = 2,
    CARD_VALUE_THREE = 3,
    CARD_VALUE_FOUR = 4,
    CARD_VALUE_FIVE = 5,
    CARD_VALUE_SIX = 6,
    CARD_VALUE_SEVEN = 7,
    CARD_VALUE_EIGHT = 8,
    CARD_VALUE_NINE = 9,
    CARD_VALUE_TEN = 10,
    CARD_VALUE_QUEEN = 11,
    CARD_VALUE_KING = 12,
    CARD_VALUE_JOKER = 13
};

typedef struct Card {
    int suit;
    int value;
} Card;

Card* card_new(int suit, int value);
void card_free(Card* card);
wchar_t card_get_char_of_value(Card* card);
wchar_t card_get_char_of_suit(Card* card);
void card_print(Card* card, int iteration);
void card_print_simple(Card* card);
Queue* card_new_queue();
void queue_card_print(Queue* queue);
void queue_card_print_simple(Queue* queue);

#endif
