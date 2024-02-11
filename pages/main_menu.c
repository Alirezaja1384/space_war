#include <ncurses.h>
#include <assert.h>
#include "../utils/menu.h"
#include "../utils/users.h"
#include "../game.h"
#include "main_menu.h"

MenuState mm_menu_state;

static void handle_exit(void *argv[], int argc);
static void handle_logout(void *argv[], int argc);
static void handle_play(void *argv[], int argc);

void init_main_menu(GameState *state_ptr)
{
    static void *state_argv[1];
    state_argv[0] = state_ptr;

    static MenuItem menu_items[] = {
        {"Play", handle_play, state_argv, 1},
        {"logout", handle_logout, state_argv, 1},
        {"Exit", handle_exit, NULL, 0},
        {NULL, NULL, NULL, 0}};

    init_menu_state(&mm_menu_state, NULL, menu_items);
}

void main_menu_handle_keys(GameState *state_ptr, int key)
{
    menu_handle_keys(&mm_menu_state, key);
}

void render_main_menu(GameState *state_ptr, WINDOW *win)
{
    wclear(win);
    render_menu(win, &mm_menu_state);
}

void destroy_main_menu(GameState *state_ptr)
{
    mm_menu_state.current_item = 0;
}

static void handle_play(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];
    state_ptr->page = PAGE_SELECT_USER; // Select second user
}

static void handle_logout(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];

    logout_user(state_ptr, USER_1);
    state_ptr->page = PAGE_SELECT_USER;
}

static void handle_exit(void *argv[], int argc)
{
    assert(argc == 0);
    exit_game();
}

PageFuncs get_main_menu_page_funcs(void)
{
    PageFuncs funcs = {
        init_main_menu,
        main_menu_handle_keys,
        NULL,
        render_main_menu,
        destroy_main_menu};

    return funcs;
}