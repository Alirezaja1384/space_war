#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "utils/tui.h"
#include "utils/colors.h"
#include "utils/assertion.h"
#include "pages/render.h"
#include "game.h"

GameState game_state;

int main(void)
{
    WINDOW *mainwin = setup_tui(MW_HEIGHT, MW_WIDTH);
    init_game();

    int inp;
    do
    {
        tick_page(mainwin, &game_state, inp);
        wrefresh(mainwin);
    } while ((inp = wgetch(mainwin)) != 27); // Until user presses ESC

    exit_game();
    return 0;
}

void init_game()
{
    game_state.page = PAGE_MAIN_MENU;
    game_state.map_path = NULL;

    strcpy(game_state.user1.username, "user1");
    strcpy(game_state.user2.username, "user2");

    game_state.user1_meta.initial_lives = 5;
    game_state.user1_meta.current_game_scores = 0;

    game_state.user2_meta.initial_lives = 5;
    game_state.user2_meta.current_game_scores = 0;
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