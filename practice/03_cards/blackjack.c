#include "blackjack.h"
#include "logger.h"
#include <stdio.h>
#include <wchar.h>

const int BLACK_JACK_MAX_SCORE = 21;
const int BLACK_JACK_DEALER_BOUND = 17;

BlackJack* black_jack_new(FILE *log_file)
{
    BlackJack* black_jack = (BlackJack*)malloc(sizeof(BlackJack));
    black_jack->desk_of_cards = card_new_queue();
    black_jack->dealer = queue_new();
    black_jack->player = queue_new();
    black_jack->log_file = log_file;
    logger_print(black_jack->log_file, "Game is started");
    return black_jack;
}

void black_jack_free(BlackJack* black_jack)
{
    queue_free(black_jack->desk_of_cards);
    queue_free(black_jack->dealer);
    queue_free(black_jack->player);
    free(black_jack);
}

int black_jack_count_score(Queue* queue)
{
    int score = 0;
    for (QueueNode* node = queue->beg->next;
         queue_is_node_dereferenable(queue, node);
         node = node->next) {
        Card* card = (Card*)node->data;
        if (card_is_number(card)) {
            score += card->value;
        } else if (card->value == CARD_VALUE_ACE) {
            if (score > 10) {
                score += 1;
            } else {
                score += 11;
            }
        } else {
            score += 10;
        }
    }
    return score;
}

int black_jack_get_verdict(BlackJack* black_jack, bool force_compute)
{
    int dealer_score = black_jack_count_score(black_jack->dealer);
    int player_score = black_jack_count_score(black_jack->player);
    if (dealer_score > BLACK_JACK_MAX_SCORE
        && player_score > BLACK_JACK_MAX_SCORE) {
        return BLACK_JACK_DRAW;
    }
    if (dealer_score > BLACK_JACK_MAX_SCORE) {
        return BLACK_JACK_PLAYER_WIN;
    }
    if (dealer_score == BLACK_JACK_MAX_SCORE) {
        return BLACK_JACK_DEALER_WIN;
    }
    if (player_score > BLACK_JACK_MAX_SCORE) {
        return BLACK_JACK_DEALER_WIN;
    }
    if (player_score == BLACK_JACK_MAX_SCORE) {
        return BLACK_JACK_PLAYER_WIN;
    }
    if (force_compute) {
        if (dealer_score == player_score) {
            return BLACK_JACK_DRAW;
        } else if (dealer_score > player_score) {
            return BLACK_JACK_DEALER_WIN;
        } else {
            return BLACK_JACK_PLAYER_WIN;
        }
    }
    return BLACK_JACK_CONTINUE;
}

bool black_jack_can_player_take_the_card(BlackJack* black_jack)
{
    return black_jack_count_score(black_jack->player)
        >= BLACK_JACK_MAX_SCORE;
}

bool black_jack_dealer_want_take_card(BlackJack* black_jack)
{
    return black_jack_count_score(black_jack->dealer)
        < BLACK_JACK_DEALER_BOUND;
}

void black_jack_take_card(BlackJack* black_jack, Queue* queue)
{
    queue_push_back(queue, queue_pop_back(black_jack->desk_of_cards));
}

void black_jack_take_by_player(BlackJack* black_jack)
{
    logger_print(black_jack->log_file, "Player take card");
    black_jack_take_card(black_jack, black_jack->player);
}

void black_jack_take_by_dealer(BlackJack* black_jack)
{
    logger_print(black_jack->log_file, "Dealer take card");
    black_jack_take_card(black_jack, black_jack->dealer);
}

void black_jack_deal_cards(BlackJack* black_jack)
{
    queue_push_back(black_jack->player,
        queue_pop_back(black_jack->desk_of_cards));
    queue_push_back(black_jack->player,
        queue_pop_back(black_jack->desk_of_cards));
    queue_push_back(black_jack->dealer,
        queue_pop_back(black_jack->desk_of_cards));
}

void black_jack_fold_all_cards(BlackJack* black_jack)
{
    logger_print(black_jack->log_file, "Fold all cards");
    while (black_jack->dealer->size > 0) {
        queue_push_back(black_jack->desk_of_cards,
            queue_pop_back(black_jack->dealer));
    }
    while (black_jack->player->size > 0) {
        queue_push_back(black_jack->desk_of_cards,
            queue_pop_back(black_jack->player));
    }
}

void black_jack_shuffle(BlackJack* black_jack)
{
    logger_print(black_jack->log_file, "Shuffle cards");
    queue_shuffle(black_jack->desk_of_cards);
}

void black_jack_print(BlackJack* black_jack)
{
    fflush(stdout);
    queue_card_print(black_jack->player);
    wprintf(L"\n");
    queue_card_print(black_jack->dealer);
    wprintf(L"\nPlayer score: %d\nDealer score: %d\n",
        black_jack_count_score(black_jack->player),
        black_jack_count_score(black_jack->dealer));
}
