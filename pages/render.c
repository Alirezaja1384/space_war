#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "../game.h"
#include "../utils/assertion.h"
#include "main_menu.h"
#include "select_map.h"
#include "gameplay/page.h"
#include "round_finished.h"
#include "game_finished.h"
#include "render.h"

PageFuncs get_page_funcs(Page page);

void tick_page(WINDOW *win, GameState *state_ptr, int key)
{
    static int last_key = ERR;
    static int last_page = -1;
    PageFuncs funcs = get_page_funcs(state_ptr->page);

    // Destroy old page and initialize new page if needed
    if (state_ptr->page != last_page)
    {
        if (last_page != -1)
        {
            PageFuncs last_page_funcs = get_page_funcs(last_page);
            if (last_page_funcs.destroy != NULL)
                last_page_funcs.destroy(state_ptr);

            // Clear last page's output
            wclear(win);
        }

        if (funcs.init != NULL)
            funcs.init(state_ptr);
    }

    // Render the page
    if (funcs.render != NULL)
        funcs.render(state_ptr, win);

    // Remember the page
    last_page = state_ptr->page;

    // Handle keypresses
    if (key != ERR && funcs.handle_keys != NULL)
    {
        if (!DISALLOW_KEY_HOLD || last_key == ERR || key != last_key)
            funcs.handle_keys(state_ptr, key);
    }

    // Sync
    if (funcs.sync != NULL)
        funcs.sync(state_ptr);

    // Remember the pressed key
    last_key = key;
}

void destroy_current_page(GameState *state_ptr)
{
    PageFuncs funcs = get_page_funcs(state_ptr->page);
    funcs.destroy(state_ptr);
}

PageFuncs get_page_funcs(Page page)
{
    switch (page)
    {
    case PAGE_MAIN_MENU:
        return get_main_menu_page_funcs();

    case PAGE_SELECT_MAP:
        return get_select_map_page_funcs();

    case PAGE_GAMEPLAY:
        return get_gameplay_page_funcs();

    case PAGE_ROUND_FINISHED:
        return get_round_finished_funcs();

    case PAGE_GAME_FINISHED:
        return get_game_finished_funcs();

    default:
        error("Invalid page!");
        break;
    }
}
