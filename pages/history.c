#include <string.h>
#include <stdbool.h>
#include "render.h"
#include "history.h"
#include "../utils/assertion.h"
#include "../utils/users.h"
#include "../utils/history.h"

static bool rendered = false;
static HistFetchResult *fetch_result = NULL;

static void init_history(GameState *state_ptr)
{
    fetch_result = get_related_hist_records(state_ptr->user1.id, HISTORY_ITEMS);
}

static void history_handle_keys(GameState *state_ptr, int key)
{
    if (key == KEY_BACKSPACE)
        state_ptr->page = PAGE_MAIN_MENU;
}

static void render_history(GameState *state_ptr, WINDOW *win)
{
    if (rendered)
        return;

    not_null(fetch_result);
    const char *pattern = "%-" MAX_USERNAME_LENGTH_STR "s %-" MAX_USERNAME_LENGTH_STR "s %s\n";

    wmove(win, 0, MAX_USERNAME_LENGTH + 3);
    wprintw(win, "History\n");
    wmove(win, 1, MAX_USERNAME_LENGTH - 7);
    wprintw(win, "Press `backspace` to go back\n\n");

    wprintw(win, pattern, "<|Player 1|>", "<|Player 2|>", "<|Result|>\n");

    for (size_t i = 0; i < fetch_result->count; i++)
    {
        HistRecord *record = fetch_result->records[i];
        char *uid = state_ptr->user1.id;

        wprintw(
            win,
            pattern,
            get_user_by_id(record->player_1_id)->username,
            get_user_by_id(record->player_2_id)->username,
            record->result == DRAW ? "DRAW"
            : (record->result == PLAYER_1_WON && strcmp(record->player_1_id, uid) == 0) ||
                    (record->result == PLAYER_2_WON && strcmp(record->player_2_id, uid) == 0)
                ? "WIN"
                : "LOSE");
    }

    rendered = true;
}

static void destroy_history(GameState *state_ptr)
{
    free_hist_fetch_result(fetch_result);
    fetch_result = NULL;
    rendered = false;
}

PageFuncs get_history_funcs(void)
{
    static PageFuncs funcs = {
        init_history,
        history_handle_keys,
        NULL,
        render_history,
        destroy_history,
    };

    return funcs;
}
