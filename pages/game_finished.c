#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
#include "../utils/history.h"
#include "../utils/assertion.h"
#include "gameplay/definations.h"
#include "render.h"
#include "game_finished.h"

// TODO: Show rounds won

static time_t restart_at;
static GameResult game_result;

static int seconds_remaining(void)
{
    return restart_at - time(NULL);
}

void init_game_finished(GameState *state_ptr)
{
    UserMeta u1_meta = state_ptr->user1_meta, u2_meta = state_ptr->user2_meta;
    game_result = u1_meta.rounds_won > u2_meta.rounds_won
                      ? PLAYER_1_WON
                  : u2_meta.rounds_won > u1_meta.rounds_won ? PLAYER_2_WON
                                                            : DRAW;
    HistRecord hist_record;
    hist_record.result = game_result;

    hist_record.player_1_id[0] = '\0';
    strncat(hist_record.player_1_id, state_ptr->user1.id, ID_LENGTH);

    hist_record.player_2_id[0] = '\0';
    strncat(hist_record.player_2_id, state_ptr->user2.id, ID_LENGTH);

    append_hist_record(hist_record);
    restart_at = time(NULL) + RESTART_GAME_IN;
}

void sync_game_finished(GameState *state_ptr)
{
    if (seconds_remaining() <= 0)
        state_ptr->page = PAGE_MAIN_MENU;
}

void render_game_finished(GameState *state_ptr, WINDOW *win)
{
    int middle_y = getmaxy(win) / 2;
    int middle_x = getmaxx(win) / 2;

    const int RESULT_MAX_LEN = 100;
    char result_text[RESULT_MAX_LEN + 1];
    attr_t result_text_color;

    switch (game_result)
    {
    case PLAYER_1_WON:
        result_text_color = PLAYER_1_COLOR_PAIR;
        snprintf(result_text, RESULT_MAX_LEN, "%s won the game!", state_ptr->user1.username);
        break;

    case PLAYER_2_WON:
        result_text_color = PLAYER_2_COLOR_PAIR;
        snprintf(result_text, RESULT_MAX_LEN, "%s won the game!", state_ptr->user2.username);
        break;

    case DRAW:
        result_text_color = DRAW_COLOR_PAIR;
        snprintf(result_text, RESULT_MAX_LEN, "Draw!");
        break;

    default:
        error("Invalid game result!");
    }

    wclear(win);
    wmove(win, middle_y - 1, middle_x - strlen(result_text) / 2);
    wattr_on(win, result_text_color, NULL);
    wprintw(win, result_text);
    wattr_off(win, result_text_color, NULL);

    wmove(win, middle_y, middle_x - 14);
    wprintw(win, "Going back home in %d seconds!", seconds_remaining());
}

static void destroy_game_finished(GameState *state_ptr)
{
    state_ptr->round_winner = -1;
    state_ptr->played_rounds = -1;
    state_ptr->target_rounds = -1;
}

PageFuncs get_game_finished_funcs(void)
{
    static PageFuncs funcs = {
        init_game_finished,
        NULL,
        sync_game_finished,
        render_game_finished,
        destroy_game_finished};

    return funcs;
}