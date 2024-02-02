#ifndef sw_utils_menu
#define sw_utils_menu

#include <stdlib.h>
#include <ncurses.h>

#define MENU_ITEM_MAX_LEN 50

typedef void (*on_select_func)(void *argv[], int argc);

typedef struct
{
    char *display;
    on_select_func on_select;
    void *argv;
    int argc;
} MenuItem;

typedef struct
{
    MenuItem *items;
    int current_item;
} MenuState;

void init_menu_state(MenuState *state_ptr, MenuItem *items);
void render_menu(WINDOW *win, MenuState *state_ptr);
void menu_handle_keys(MenuState *state_ptr, int key);

#endif
