#include <errno.h>
#include <unistd.h>
#include <form.h>
#include <ncurses.h>
#include <assert.h>
#include <string.h>
#include "../utils/tui.h"
#include "../utils/colors.h"
#include "../utils/menu.h"
#include "../utils/form.h"
#include "../utils/users.h"
#include "../utils/string.h"
#include "../utils/assertion.h"
#include "../limits.h"
#include "../game.h"
#include "select_user.h"

static MenuState su_menu_state;
static UserIndex target_user_index;
FIELD *create_user_fields[3];

static void init_users_menu(GameState *state_ptr);
static void handle_user_select(void *argv[], int argc);
static void handle_back(void *argv[], int argc);
static void handle_exit(void *argv[], int argc);
static void handle_user_create(void *argv[], int argc);
static void handle_create_user_submit(GameState *state_ptr, char *username);

static void init_select_user(GameState *state_ptr)
{
    target_user_index = state_ptr->user1_meta.signed_in ? USER_2 : USER_1;
    init_users_menu(state_ptr);
}

static void select_user_handle_keys(GameState *state_ptr, int key)
{
    menu_handle_keys(&su_menu_state, key);
}

static void render_select_user(GameState *state_ptr, WINDOW *win)
{
    wclear(win);
    render_menu(win, &su_menu_state);
}

static void destroy_select_user(GameState *state_ptr)
{
    su_menu_state.current_item = 0;
}

PageFuncs get_select_user_funcs(void)
{
    PageFuncs funcs = {
        init_select_user,
        select_user_handle_keys,
        NULL,
        render_select_user,
        destroy_select_user};

    return funcs;
}

static void init_users_menu(GameState *state_ptr)
{

    static void *user_argvs[MAX_USERS][2];
    static MenuItem menu_items[MAX_USERS + 1];

    int users_count = 0;
    UserNode *users_head = get_users_head();
    char *excluded_id = target_user_index == USER_2 ? state_ptr->user1.id : NULL;

    while (users_count < MAX_USERS && users_head != NULL)
    {
        // Add user item if not excluded
        if (excluded_id == NULL || strcmp(excluded_id, users_head->user->id) != 0)
        {
            user_argvs[users_count][0] = (void *)state_ptr;
            user_argvs[users_count][1] = (void *)users_head->user;

            menu_items[users_count] = (MenuItem){
                users_head->user->username,
                handle_user_select,
                user_argvs[users_count],
                2};

            users_count++;
        }

        users_head = users_head->next;
    }

    static void *state_argv[1];
    state_argv[0] = (void *)state_ptr;

    menu_items[users_count] = (MenuItem){"<| Create new user |>", handle_user_create, state_argv, 1};

    if (target_user_index == USER_1)
        menu_items[users_count + 1] = (MenuItem){"<| Exit game |>", handle_exit, NULL, 0};
    else
        menu_items[users_count + 1] = (MenuItem){"<| Back to main menu |>", handle_back, state_argv, 1};

    menu_items[users_count + 2] = (MenuItem){NULL, NULL, NULL, 0};

    init_menu_state(&su_menu_state, target_user_index == USER_1 ? "Select the primary user:" : "Select the opponent:", menu_items);
}

static void handle_user_select(void *argv[], int argc)
{
    assert(argc == 2);
    GameState *state_ptr = argv[0];
    User *selected_user = argv[1];

    login_user(state_ptr, target_user_index, selected_user);

    if (target_user_index == USER_1)
        state_ptr->page = PAGE_MAIN_MENU;
    else
        state_ptr->page = PAGE_SELECT_MAP;
}

static void handle_back(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];

    state_ptr->page = PAGE_MAIN_MENU;
}

static void handle_exit(void *argv[], int argc)
{
    assert(argc == 0);
    exit_game();
}

static bool render_user_create(WINDOW *modal)
{
    wclear(modal);
    box(modal, 0, 0);
    mvwprintw(modal, 1, 7, "Create new user");
    return wgetch(modal) == 27;
}

static void handle_user_create(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];

    int m_height = MW_HEIGHT / 4, m_width = MW_WIDTH / 3;

    FIELD *username_label = label_field("Enter username (Characters/digits):", 1, m_width - 4, 2, 2);
    FIELD *username_field = not_null(new_field(1, m_width - 4, 3, 2, 0, 0));
    set_field_opts(username_field, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_back(username_field, COLOR_PAIR(CP_BLACK_CYAN) | A_UNDERLINE);
    set_field_type(username_field, TYPE_REGEXP, USERNAME_REGEX);

    create_user_fields[0] = username_label;
    create_user_fields[1] = username_field;
    create_user_fields[2] = NULL;

    FORM *form = not_null(new_form(create_user_fields));

    WINDOW *modal_win = init_centered_win(m_height, m_width);
    WINDOW *form_win = not_null(derwin(modal_win, 4, m_width - 2, 1, 1));

    curs_set(1);

    set_form_win(form, form_win);
    set_form_sub(form, modal_win);
    post_form(form);

    apply_modal_style(modal_win, CP_BLACK_CYAN, "Create new user!");
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
        handle_create_user_submit(state_ptr, username);
    }

    unpost_form(form);
    destroy_win(form_win);
    destroy_win(modal_win);
}

static void handle_create_user_submit(GameState *state_ptr, char *username)
{
    create_user(username);
    init_users_menu(state_ptr);
}
