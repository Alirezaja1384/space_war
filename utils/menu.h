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
    void **argv;
    int argc;
} MenuItem;

typedef struct
{
    char *title;
    MenuItem *items;
    int current_item;
} MenuState;

void init_menu_state(MenuState *state_ptr, char *title, MenuItem *items);
void render_menu(WINDOW *win, MenuState *state_ptr);
void menu_handle_keys(MenuState *state_ptr, int key);

void fill_menu_item(MenuItem *item_ptr, char *display, on_select_func on_select, void **argv, int argc);

#endif
