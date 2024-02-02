#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "../game.h"
#include "../utils/assertion.h"
#include "main_menu.h"
#include "select_map.h"
#include "render.h"

PageFuncs get_page_funcs(Page page);

void tick_page(WINDOW *win, GameState *state_ptr, int key)
{
    static int last_page = -1;
    PageFuncs funcs = get_page_funcs(state_ptr->page);

    // Destroy old page and initialize new page if needed
    if (state_ptr->page != last_page)
    {
        if (last_page != -1)
        {
            PageFuncs last_page_funcs = get_page_funcs(last_page);
            if (last_page_funcs.destroy != NULL)
                last_page_funcs.destroy();
        }

        if (funcs.init != NULL)
            funcs.init(state_ptr);
    }

    if (funcs.render != NULL)
        // Render the page
        funcs.render(win);

    // Remember the last page
    last_page = state_ptr->page;

    // Handle keypresses
    if (key != ERR && funcs.handle_keys != NULL)
        funcs.handle_keys(state_ptr, key);
}

PageFuncs get_page_funcs(Page page)
{
    switch (page)
    {
    case PAGE_MAIN_MENU:
        PageFuncs main_menu_funcs = {
            init_main_menu,
            main_menu_handle_keys,
            render_main_menu,
            destroy_main_menu};

        return main_menu_funcs;

    case PAGE_SELECT_MAP:
        PageFuncs select_map_funcs = {
            init_select_maps,
            select_maps_handle_keys,
            render_select_maps,
            destroy_select_maps};

        return select_map_funcs;

    default:
        error("Invalid page!");
        break;
    }
}
