#include <stdio.h>
#include <wchar.h>

#include "console.h"
#include "girl_art.h"

const wchar_t* GIRL_ART_FRAME_1
    = L"                               _.._\n"
      L"                             .'    '.\n"
      L"                            (____/`\\ \\\n"
      L"                           (  |' ' )  )\n"
      L"                           )  _\\= _/  (\n"
      L"                 __..---.(`_.'  ` \\    )\n"
      L"                `;-\"-._(_( .      `;  (\n"
      L"                /       `-`'--'     ; )\n"
      L"               /    /  .    ( .  ,| |(\n"
      L"_.-`'---...__,'    /-,..___.-'--'_| |_)\n"
      L"'-'``'-.._       ,'  |   / .........'\n"
      L"          ``;--\"`;   |   `-`\n"
      L"             `'..__.'";
const wchar_t* GIRL_ART_FRAME_2
    = L"                               _.._\n"
      L"                             .'    '.\n"
      L"                            (____/`\\ \\\n"
      L"                            ) |' ' )  )\n"
      L"                           (  _\\= _/  (\n"
      L"                 __..---.(`_.'  ` \\    (\n"
      L"                `;-\"-._(_( .      `;  )\n"
      L"                /       `-`'--'     ; (\n"
      L"               /    /  .    ( .  ,| |)\n"
      L"_.-`'---...__,'    /-,..___.-'--'_| |_(\n"
      L"'-'``'-.._       ,'  |   / .........'\n"
      L"          ``;--\"`;   |   `-`\n"
      L"             `'..__.'";
const wchar_t* GIRL_ART_FRAME_3
    = L"                               _.._\n"
      L"                             .'    '.\n"
      L"                            (____/`\\ \\\n"
      L"                           (  |' ' )  )\n"
      L"                           )  _\\= _/   (\n"
      L"                 __..---.(`_.'  ` \\     (\n"
      L"                `;-\"-._(_( .      `;   )\n"
      L"                /       `-`'--'     ;  (\n"
      L"               /    /  .    ( .  ,| | )\n"
      L"_.-`'---...__,'    /-,..___.-'--'_| |__(\n"
      L"'-'``'-.._       ,'  |   / .........'\n"
      L"          ``;--\"`;   |   `-`\n"
      L"             `'..__.'";
const wchar_t* GIRL_ART_FRAME_4
    = L"                               _.._\n"
      L"                             .'    '.\n"
      L"                            (____/`\\ \\\n"
      L"                           (  |' _ )  )\n"
      L"                           )  _\\= _/  (\n"
      L"                 __..---.(`_.'  ` \\    )\n"
      L"                `;-\"-._(_( .      `;  (\n"
      L"                /       `-`'--'     ; )\n"
      L"               /    /  .    ( .  ,| |(\n"
      L"_.-`'---...__,'    /-,..___.-'--'_| |_)\n"
      L"'-'``'-.._       ,'  |   / .........'\n"
      L"          ``;--\"`;   |   `-`\n"
      L"             `'..__.'";

void girl_art_show()
{
    console_clear();
    const int count_of_frames = 3;
    const wchar_t* girl_art_frames[3]
        = { GIRL_ART_FRAME_1, GIRL_ART_FRAME_2, GIRL_ART_FRAME_3 };
    for (int i = 0; i < 5; ++i) {
        for (int frame = 0; frame < count_of_frames; ++frame) {
            console_clear();
            wprintf(girl_art_frames[frame]);
            fflush(stdout);
            console_sleep(250000);
        }
    }
    console_sleep(200000);
    console_clear();
    wprintf(GIRL_ART_FRAME_4);
    fflush(stdout);
    console_sleep(250000);
    console_clear();
    wprintf(GIRL_ART_FRAME_1);
    fflush(stdout);
    console_sleep(800000);
}
