#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "menu.h"
#include "colors.h"
#include "assertion.h"

void init_menu_state(MenuState *state_ptr, MenuItem *items)
{
    for (int i = 0; items[i].display != NULL; i++)
        if (strlen(items[i].display) > MENU_ITEM_MAX_LEN)
            error("Out of range menu item!");

    state_ptr->items = items;
    state_ptr->current_item = 0;
};

void render_menu(WINDOW *win, MenuState *state_ptr)
{
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
        void (*on_select)(void *argv[], int argc) = state_ptr->items[state_ptr->current_item].on_select;

        MenuItem current_item = state_ptr->items[state_ptr->current_item];
        if (on_select != NULL)
            on_select(current_item.argv, current_item.argc);

        break;
    }
};
