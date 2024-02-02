#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "../game.h"
#include "../utils/assertion.h"
#include "main_menu.h"
#include "select_map.h"

void tick_page(WINDOW *win, GameState *state_ptr, int key)
{
    static int last_page = -1;

    // Handle keypresses
    if (key != ERR)
    {
        switch (state_ptr->page)
        {
        case PAGE_MAIN_MENU:
            main_menu_handle_keys(state_ptr, key);
            break;

        case PAGE_SELECT_MAP:
            select_maps_handle_keys(state_ptr, key);
            break;
        }
    }

    // Destroy old page and initialize new page if needed
    if (state_ptr->page != last_page)
    {
        // Destroy last page
        switch (last_page)
        {
        case PAGE_MAIN_MENU:
            destroy_main_menu();
            break;

        case PAGE_SELECT_MAP:
            destroy_select_maps();
            break;
        }

        // Initialize new page
        switch (state_ptr->page)
        {
        case PAGE_MAIN_MENU:
            init_main_menu(state_ptr);
            break;

        case PAGE_SELECT_MAP:
            init_select_maps(state_ptr);
            break;
        }
    }

    // Render the page
    switch (state_ptr->page)
    {
    case PAGE_MAIN_MENU:
        render_main_menu(win);
        break;

    case PAGE_SELECT_MAP:
        render_select_maps(win);
        break;

    default:
        error("Invalid page!");
        break;
    }

    // Remember the last page
    last_page = state_ptr->page;
}