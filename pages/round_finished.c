#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
#include "./gameplay/definations.h"
#include "render.h"
#include "round_finished.h"

time_t next_round;

static int seconds_remaining(void)
{
    return next_round - time(NULL);
}

void init_round_finished(GameState *state_ptr)
{
    next_round = time(NULL) + NEXT_ROUND_IN_SECONDS;
}

void sync_round_finished(GameState *state_ptr)
{
    if (seconds_remaining() <= 0)
        state_ptr->page = PAGE_SELECT_MAP;
}

void render_round_finished(GameState *state_ptr, WINDOW *win)
{
    int middle_y = getmaxy(win) / 2;
    int middle_x = getmaxx(win) / 2;

    char *winner_username = state_ptr->round_winner == 1 ? state_ptr->user1.username : state_ptr->user2.username;
    attr_t winner_color = state_ptr->round_winner == 1 ? PLAYER_1_COLOR_PAIR : PLAYER_2_COLOR_PAIR;

    wclear(win);
    wmove(win, middle_y - 1, middle_x - strlen(winner_username) / 2 - 8);
    wattr_on(win, winner_color, NULL);
    wprintw(win, "%s", winner_username);
    wattr_off(win, winner_color, NULL);
    wprintw(win, " Won this round!");

    wmove(win, middle_y, middle_x - 18);
    wprintw(win, "Next round will start in %d seconds!", seconds_remaining());
}

static void destroy_round_finished(GameState *state_ptr)
{
    state_ptr->round_winner = -1;
}

PageFuncs get_round_finished_funcs(void)
{
    static PageFuncs funcs = {
        init_round_finished,
        NULL,
        sync_round_finished,
        render_round_finished,
        destroy_round_finished};

    return funcs;
}