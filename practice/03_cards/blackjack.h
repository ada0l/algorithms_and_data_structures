#ifndef BLACKJACK_HEADER_FILE_H
#define BLACKJACK_HEADER_FILE_H

#include <stdbool.h>

#include "card.h"
#include "queue.h"

extern const int BLACK_JACK_MAX_SCORE;
extern const int BLACK_JACK_DEALER_BOUND;

enum BLACK_JACK_VERDICT {
    BLACK_JACK_DEALER_WIN,
    BLACK_JACK_PLAYER_WIN,
    BLACK_JACK_DRAW,
    BLACK_JACK_CONTINUE
};

typedef struct BlackJack {
    Queue* desk_of_cards;
    Queue* dealer;
    Queue* player;
} BlackJack;

BlackJack* black_jack_new();

void black_jack_free(BlackJack* black_jack);

int black_jack_count_score(Queue* queue);

int black_jack_get_verdict(BlackJack* black_jack, bool force_compute);

bool black_jack_can_player_take_the_card(BlackJack* black_jack);

bool black_jack_dealer_want_take_card(BlackJack* black_jack);

void black_jack_take_card(BlackJack* black_jack, Queue* queue);

void black_jack_take_by_player(BlackJack* black_jack);

void black_jack_take_by_dealer(BlackJack* black_jack);

void black_jack_deal_cards(BlackJack* black_jack);

void black_jack_fold_all_cards(BlackJack* black_jack);

void black_jack_shuffle(BlackJack* black_jack);

void black_jack_print(BlackJack* black_jack);

#endif
