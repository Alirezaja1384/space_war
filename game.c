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
    game_state.round_winner = -1;
    game_state.target_rounds = 2;
    game_state.played_rounds = 0;

    strcpy(game_state.user1.id, "1af910b4-9d6e-4512-b680-2fc90513a2b4");
    strcpy(game_state.user1.username, "user1");
    game_state.user1_meta = (UserMeta){false, 5, 0};

    strcpy(game_state.user1.id, "adf6e0a7-7f23-4742-966f-9470c5c5239a");
    strcpy(game_state.user2.username, "user2");
    game_state.user2_meta = (UserMeta){false, 5, 0};
}

void exit_game(void)
{
    destroy_current_page(&game_state);

    WINDOW *win = get_win();
    wclear(win);
    wprintw(win, "Bye ...");
    wrefresh(win);
    sleep(1);

    destroy_tui();
    exit(0);
}