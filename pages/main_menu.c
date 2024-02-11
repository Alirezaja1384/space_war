#include <ncurses.h>
#include <form.h>
#include <assert.h>
#include <string.h>
#include "../utils/tui.h"
#include "../utils/menu.h"
#include "../utils/string.h"
#include "../utils/users.h"
#include "../utils/form.h"
#include "../utils/assertion.h"
#include "../limits.h"
#include "../game.h"
#include "main_menu.h"

static MenuState mm_menu_state;
static FIELD *edit_username_fields[3];

static void handle_exit(void *argv[], int argc);
static void handle_history(void *argv[], int argc);
static void handle_logout(void *argv[], int argc);
static void handle_play(void *argv[], int argc);

static void handle_change_username(void *argv[], int argc);
static void handle_change_username_submit(GameState *state_ptr, char *new_username);

void init_main_menu(GameState *state_ptr)
{
    static void *state_argv[1];
    state_argv[0] = state_ptr;

    static MenuItem menu_items[] = {
        {"Play", handle_play, state_argv, 1},
        {"History", handle_history, state_argv, 1},
        {"Change username", handle_change_username, state_argv, 1},
        {"Logout", handle_logout, state_argv, 1},
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

static void handle_history(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];

    state_ptr->page = PAGE_HISTORY;
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

static void handle_change_username(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];

    int m_height = MW_HEIGHT / 4, m_width = MW_WIDTH / 3;

    FIELD *username_label = label_field("Enter username (Characters/digits):", 1, m_width - 4, 2, 2);
    FIELD *username_field = not_null(new_field(1, m_width - 4, 3, 2, 0, 0));
    set_field_opts(username_field, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_back(username_field, COLOR_PAIR(CP_BLACK_CYAN) | A_UNDERLINE);
    set_field_buffer(username_field, 0, state_ptr->user1.username);
    set_field_type(username_field, TYPE_REGEXP, USERNAME_REGEX);

    edit_username_fields[0] = username_label;
    edit_username_fields[1] = username_field;
    edit_username_fields[2] = NULL;

    FORM *form = not_null(new_form(edit_username_fields));

    WINDOW *modal_win = init_centered_win(m_height, m_width);
    WINDOW *form_win = not_null(derwin(modal_win, 4, m_width - 2, 1, 1));

    curs_set(1);

    set_form_win(form, form_win);
    set_form_sub(form, modal_win);
    post_form(form);

    apply_modal_style(modal_win, CP_BLACK_CYAN, "Edit you username!");
    wrefresh(form_win);
    wrefresh(modal_win);
    refresh();

    int key;
    bool submitted = false;
    while (!((key = wgetch(modal_win)) == 27 || (submitted = form_handle_keys(form, key, NULL))))
    {
    }

    curs_set(0);

    if (submitted)
    {
        char *username = trim_whitespaces(field_buffer(username_field, 0));
        handle_change_username_submit(state_ptr, username);
    }

    unpost_form(form);
    destroy_win(form_win);
    destroy_win(modal_win);
}

static void handle_change_username_submit(GameState *state_ptr, char *new_username)
{
    User *current_user = not_null(get_user_by_id(state_ptr->user1.id));

    if (change_username(current_user, new_username))
        apply_user_changes(state_ptr);
}
