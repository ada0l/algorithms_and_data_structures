#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "blackjack.h"
#include "card.h"
#include "console.h"
#include "queue.h"

#if !_WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

// time in seconds
void _sleep_(int time)
{
#if !_WIN32
    sleep(time);
#else
    Sleep(time * 1000);
#endif
}

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.utf8");
    BlackJack* black_jack = black_jack_new();
    while (true) {
        black_jack_fold_all_cards(black_jack);
        black_jack_shuffle(black_jack);
        black_jack_deal_cards(black_jack);
        console_clear();
        black_jack_print(black_jack);
        while (true) {
            if (console_yes_or_no("Do you want to take card?")) {
                black_jack_take_by_player(black_jack);
                console_clear();
                black_jack_print(black_jack);
            } else {
                break;
            }
        }

        console_clear();
        black_jack_print(black_jack);

        while (black_jack_dealer_want_take_card(black_jack)) {
            _sleep_(1);
            black_jack_take_by_dealer(black_jack);
            console_clear();
            black_jack_print(black_jack);
        }

        console_clear();
        black_jack_print(black_jack);

        int verdict = black_jack_get_verdict(black_jack, true);
        switch (verdict) {
        case BLACK_JACK_DEALER_WIN:
            wprintf(L"Dealer is win\n");
            break;
        case BLACK_JACK_PLAYER_WIN:
            wprintf(L"Player is win\n");
            break;
        case BLACK_JACK_DRAW:
            wprintf(L"Draw\n");
            break;
        }

        if (console_yes_or_no("Do you want to play again?") == 0) {
            break;
        }
    }

    black_jack_free(black_jack);
    return EXIT_SUCCESS;
}
