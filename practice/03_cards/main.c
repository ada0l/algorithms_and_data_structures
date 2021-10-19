#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "blackjack.h"
#include "console.h"
#include "girl_art.h"
#include "logger.h"

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.utf8");
    FILE* file = fopen("black_jack.log", "a");
    if (file == NULL) {
        wprintf(L"I can't open black_jack.log");
        exit(EXIT_FAILURE);
    }
    BlackJack* black_jack = black_jack_new(file);
    girl_art_show();
    while (true) {
        black_jack_fold_all_cards(black_jack);
        black_jack_shuffle(black_jack);
        black_jack_deal_cards(black_jack);
        console_clear();
        black_jack_print(stdout, black_jack);
        while (true) {
            if (black_jack_can_player_take_the_card(black_jack)) {
                break;
            }
            if (console_yes_or_no(L"Do you want to take card?")) {
                black_jack_take_by_player(black_jack);
                console_clear();
                black_jack_print(stdout, black_jack);
            } else {
                break;
            }
        }

        console_clear();
        black_jack_print(stdout, black_jack);

        while (black_jack_get_verdict(black_jack, false)
                == BLACK_JACK_CONTINUE
            && black_jack_dealer_want_take_card(black_jack)) {
            console_sleep(1000000);
            black_jack_take_by_dealer(black_jack);
            console_clear();
            black_jack_print(stdout, black_jack);
        }

        console_clear();
        black_jack_print(stdout, black_jack);

        int verdict = black_jack_get_verdict(black_jack, true);
        char* asd;
        switch (verdict) {
        case BLACK_JACK_DEALER_WIN:
            wprintf(L"Dealer is win %lc\n", 0x1F62D);
            break;
        case BLACK_JACK_PLAYER_WIN:
            wprintf(L"Player is win %lc\n", 0x1F680);
            break;
        case BLACK_JACK_DRAW:
            wprintf(L"Draw\n");
            break;
        }

        if (console_yes_or_no(L"Do you want to play again?") == 0) {
            break;
        }
    }

    black_jack_free(black_jack);
    return EXIT_SUCCESS;
}
