#include <ncurses.h>
#include <assert.h>
#include "../utils/menu.h"
#include "../game.h"

MenuState mm_menu_state;

void handle_exit(void *argv[], int argc);
void handle_play(void *argv[], int argc);

void init_main_menu(GameState *state_ptr)
{
    static void *play_argv[1];
    play_argv[0] = state_ptr;

    static MenuItem menu_items[] = {
        {"Play", handle_play, play_argv, 1},
        {"Exit", handle_exit, NULL, 0},
        {NULL, NULL, NULL, 0}};

    init_menu_state(&mm_menu_state, menu_items);
}

void main_menu_handle_keys(GameState *state_ptr, int key)
{
    menu_handle_keys(&mm_menu_state, key);
}

void render_main_menu(WINDOW *win)
{
    render_menu(win, &mm_menu_state);
}

void destroy_main_menu()
{
    mm_menu_state.current_item = 0;
}

void handle_exit(void *argv[], int argc)
{
    assert(argc == 0);
    exit_game();
}

void handle_play(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];
    state_ptr->page = PAGE_SELECT_MAP;
}
