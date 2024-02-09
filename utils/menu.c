#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "menu.h"
#include "colors.h"
#include "assertion.h"

void init_menu_state(MenuState *state_ptr, char *title, MenuItem *items)
{
    state_ptr->title = title;

    for (int i = 0; items[i].display != NULL; i++)
        if (strlen(items[i].display) > MENU_ITEM_MAX_LEN)
            error("Out of range menu item!");

    state_ptr->items = items;
    state_ptr->current_item = 0;
};

void render_menu(WINDOW *win, MenuState *state_ptr)
{
    if (state_ptr->title != NULL)
        wprintw(win, "%s\n", state_ptr->title);

    for (int i = 0; state_ptr->items[i].display != NULL; i++)
    {
        // Colorize If the item is selected
        if (i == state_ptr->current_item)
            wattr_on(win, COLOR_PAIR(CP_WHITE_BLUE), NULL);

        // sprintf the item text
        char item_txt[MENU_ITEM_MAX_LEN + 4]; // 3 more chars + '\0'
        sprintf(item_txt, " > %s", state_ptr->items[i].display);

        // Write the item's line
        wprintw(win, item_txt);
        for (int j = 0; j < getmaxx(win) - strlen(item_txt); j++)
            wprintw(win, " ");

        // End coloring
        wattr_off(win, COLOR_PAIR(CP_WHITE_BLUE), NULL);
    }
};

void menu_handle_keys(MenuState *state_ptr, int key)
{
    switch (key)
    {
    case KEY_UP:
        if (state_ptr->current_item > 0)
            state_ptr->current_item--;
        break;

    case KEY_DOWN:
        if (state_ptr->items[state_ptr->current_item + 1].display != NULL)
            state_ptr->current_item++;
        break;

    case '\n':
        on_select_func on_select = state_ptr->items[state_ptr->current_item].on_select;
        if (on_select != NULL)
        {
            MenuItem current_item = state_ptr->items[state_ptr->current_item];
            on_select(current_item.argv, current_item.argc);
        }

        break;
    }
};

void fill_menu_item(MenuItem *item_ptr, char *display, on_select_func on_select, void **argv, int argc)
{
    item_ptr->display = display;
    item_ptr->on_select = on_select;
    item_ptr->argv = argv;
    item_ptr->argc = argc;
}