#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include "utils/tui.h"
#include "utils/colors.h"
#include "utils/assertion.h"
#include "pages/render.h"
#include "game.h"

GameState game_state;

int main(void)
{
    WINDOW *mainwin = setup_tui();
    init_game();

    int inp;
    do
    {
        wclear(mainwin);
        tick_page(mainwin, &game_state, inp);
        wrefresh(mainwin);
    } while ((inp = wgetch(mainwin)) != 27); // Until user presses ESC

    exit_game();
    return 0;
}

void init_game()
{
    game_state.page = PAGE_MAIN_MENU;
}

void exit_game(void)
{
    WINDOW *win = get_win();
    wclear(win);
    wprintw(win, "Bye ...");
    wrefresh(win);
    sleep(1);

    destroy_tui();
    exit(0);
}