#include "blackjack.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>

const int BLACK_JACK_MAX_SCORE = 21;
const int BLACK_JACK_DEALER_BOUND = 17;

BlackJack* black_jack_new(FILE* log_file)
{
    BlackJack* black_jack = (BlackJack*)malloc(sizeof(BlackJack));
    black_jack->desk_of_cards = card_new_queue();
    black_jack->dealer = queue_new();
    black_jack->player = queue_new();
    black_jack->log_file = log_file;
    logger_print(black_jack->log_file, L"BlackJack is started", NULL);
    return black_jack;
}

void black_jack_free(BlackJack* black_jack)
{
    queue_free(black_jack->desk_of_cards);
    queue_free(black_jack->dealer);
    queue_free(black_jack->player);
    logger_print(black_jack->log_file, L"BlackJack is stoped", NULL);
    fflush(black_jack->log_file);
    fclose(black_jack->log_file);
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

int _black_jack_get_verdict(BlackJack* black_jack, bool force_compute)
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

int black_jack_get_verdict(BlackJack* black_jack, bool force_compute)
{
    int verdict = _black_jack_get_verdict(black_jack, force_compute);
    switch (verdict) {
    case BLACK_JACK_DRAW:
        logger_print(black_jack->log_file, L"Verdict: Draw", NULL);
    case BLACK_JACK_PLAYER_WIN:
        logger_print(
            black_jack->log_file, L"Verdict: Player win", NULL);
    case BLACK_JACK_DEALER_WIN:
        logger_print(
            black_jack->log_file, L"Verdict: Dealer win", NULL);
    case BLACK_JACK_CONTINUE:
        logger_print(
            black_jack->log_file, L"Verdict: Game continue", NULL);
    }
    return verdict;
}

bool black_jack_can_player_take_the_card(BlackJack* black_jack)
{
    return black_jack_count_score(black_jack->player)
        >= BLACK_JACK_MAX_SCORE;
}

bool black_jack_dealer_want_take_card(BlackJack* black_jack)
{
    bool answer = black_jack_count_score(black_jack->dealer)
        < BLACK_JACK_DEALER_BOUND;
    logger_print(black_jack->log_file,
        (answer) ? L"Dealer want to take card"
                 : L"Dealer dont want to take card",
        NULL);
    return answer;
}

void black_jack_take_card(BlackJack* black_jack, Queue* queue)
{
    queue_push_back(queue, queue_pop_back(black_jack->desk_of_cards));
}

void black_jack_take_by_player(BlackJack* black_jack)
{
    black_jack_take_card(black_jack, black_jack->player);
    Card* card = black_jack->player->end->prev->data;
    wchar_t* card_wchar_t_ptr = card_to_wchar_t_ptr(card);
    logger_print(black_jack->log_file, L"Player take card: ",
        card_wchar_t_ptr, NULL);
    free(card_wchar_t_ptr);
}

void black_jack_take_by_dealer(BlackJack* black_jack)
{
    black_jack_take_card(black_jack, black_jack->dealer);
    Card* card = black_jack->dealer->end->prev->data;
    wchar_t* card_wchar_t_ptr = card_to_wchar_t_ptr(card);
    logger_print(black_jack->log_file, L"Dealer take card: ",
        card_wchar_t_ptr, NULL);
    free(card_wchar_t_ptr);
}

void black_jack_deal_cards(BlackJack* black_jack)
{
    black_jack_take_by_player(black_jack);
    black_jack_take_by_player(black_jack);
    black_jack_take_by_dealer(black_jack);
}

void black_jack_fold_all_cards(BlackJack* black_jack)
{
    logger_print(black_jack->log_file, L"Fold all cards", NULL);
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
    logger_print(black_jack->log_file, L"Shuffle cards", NULL);
    queue_shuffle(black_jack->desk_of_cards);
}

void black_jack_print(FILE* file, BlackJack* black_jack)
{
    fflush(stdout);
    queue_card_print(file, black_jack->player);
    fwprintf(file, L"\n");
    queue_card_print(file, black_jack->dealer);
    wprintf(L"\nPlayer score: %d\nDealer score: %d\n",
        black_jack_count_score(black_jack->player),
        black_jack_count_score(black_jack->dealer));
}
