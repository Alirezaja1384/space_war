#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <form.h>
#include <ncurses.h>
#include "../../utils/tui.h"
#include "../../utils/form.h"
#include "../../utils/colors.h"
#include "../../utils/string.h"
#include "../../utils/assertion.h"
#include "../../utils/coordinates.h"
#include "../render.h"
#include "definations.h"
#include "utils.h"
#include "sync.h"
#include "page.h"

static WINDOW *header_win = NULL;
static GameplayState gameplay_state;
static FIELD *game_conf_fields[7];

static const Field_Options FIELD_OPTS = O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE;
static const chtype FIELDS_BACKGROUND = COLOR_PAIR(CP_BLACK_CYAN) | A_UNDERLINE;

static void load_map(char *map_path);
static void init_players(GameState *state_ptr);

static void render_header(GameState *state_ptr, WINDOW *win);
static void render_header_game_stats(GameState *state_ptr, WINDOW *win);
static void render_header_player_stats(GameState *state_ptr, WINDOW *win);

static void draw_cell(WINDOW *win, Coordinates pos);

static void draw_bullet(WINDOW *win, Bullet *bull);
static void draw_player(WINDOW *win, Player *player, attr_t color);
static void draw_aitem(WINDOW *win, AcquirableItem *aitem_ptr);

static void prompt_game_conf(GameState *state_ptr);

static void init_gameplay(GameState *state_ptr)
{
    assert(state_ptr->map_path != NULL);
    assert(state_ptr->user1_meta.signed_in);
    assert(state_ptr->user2_meta.signed_in);

    if (state_ptr->played_rounds < 1)
        prompt_game_conf(state_ptr);

    // Initialize map and acquirable items
    load_map(state_ptr->map_path);
    state_ptr->map_path = NULL;

    // Initiate map's mask (Don't hide any item)
    for (int j = 0; j < MAP_HEIGHT; j++)
        for (int i = 0; i < MAP_WIDTH; i++)
            gameplay_state.map_mask[j][i] = 1;

    // initialize player_1 and player_2
    init_players(state_ptr);

    // Initialize bullets
    for (int i = 0; i < MAX_ACTIVE_BULLETS; i++)
        init_bullet(gameplay_state.bullets + i);

    // Create header window
    header_win = init_header_win(MAP_HEIGHT, MAP_WIDTH);

    // Set sync needed
    gameplay_state.sync_needed = true;
}

static void gameplay_handle_keys(GameState *state_ptr, int key)
{
    bool valid_key = true;

    switch (key)
    {
        //               <Player 1 keys>
    case K_PLAYER_1_UP:
        move_player(&gameplay_state.player_1, UP);
        break;

    case K_PLAYER_1_RIGHT:
        move_player(&gameplay_state.player_1, RIGHT);
        break;

    case K_PLAYER_1_DOWN:
        move_player(&gameplay_state.player_1, DOWN);
        break;

    case K_PLAYER_1_LEFT:
        move_player(&gameplay_state.player_1, LEFT);
        break;

    case K_PLAYER_1_SHOOT:
        fire_bullet(&gameplay_state, &gameplay_state.player_1);
        break;
    //                  </Player 1 keys>

    //                  <Player 2 keys>
    case K_PLAYER_2_UP:
        move_player(&gameplay_state.player_2, UP);
        break;

    case K_PLAYER_2_RIGHT:
        move_player(&gameplay_state.player_2, RIGHT);
        break;

    case K_PLAYER_2_DOWN:
        move_player(&gameplay_state.player_2, DOWN);
        break;

    case K_PLAYER_2_LEFT:
        move_player(&gameplay_state.player_2, LEFT);
        break;

    case K_PLAYER_2_SHOOT:
        fire_bullet(&gameplay_state, &gameplay_state.player_2);
        break;
        //              </Player 2 keys>

    default:
        valid_key = false;
    }

    // Remember to sync after each key
    gameplay_state.sync_needed = valid_key;
};

static void gameplay_sync(GameState *state_ptr)
{
    gameplay_state.sync_needed = true;

    // TODO: use threads!
    // TODO: mutext to prevent moving bullets during gap between sync and render!
    for (int i = 0; i < MAX_ACTIVE_BULLETS; i++)
    {
        Bullet *bull = gameplay_state.bullets + i;
        move_bullet(bull);
    }

    if (gameplay_state.sync_needed)
    {
        sync_gameplay_states(state_ptr, &gameplay_state);
        gameplay_state.sync_needed = false;
    }
}

static void render_gameplay(GameState *state_ptr, WINDOW *win)
{
    render_header(state_ptr, header_win);

    for (int j = 0; j < MAP_HEIGHT; j++)
    {
        for (int i = 0; i < MAP_WIDTH; i++)
        {
            wmove(win, j, i);
            draw_cell(win, (Coordinates){j, i});
        }
    }
}

static void destroy_gameplay(GameState *state_ptr)
{
    // Get rid of header window
    destroy_win(header_win);
    header_win = NULL;
}

PageFuncs get_gameplay_page_funcs(void)
{
    return (PageFuncs){
        init_gameplay,
        gameplay_handle_keys,
        gameplay_sync,
        render_gameplay,
        destroy_gameplay,
    };
}

static void load_map(char *map_path)
{
    gameplay_state.aitems_count = 0;

    FILE *fp = fopen(map_path, "r");
    check(fread(&gameplay_state.map, sizeof(Map), 1, fp) == 1, "Unable to read map!");

    while (gameplay_state.aitems_count < MAX_ACQUIRABLE_ITEMS)
    {
        AcquirableItem *aitem = gameplay_state.aitems + gameplay_state.aitems_count;
        if (fread(aitem, sizeof(AcquirableItem), 1, fp) == 1)
            gameplay_state.aitems_count++;
        else
            break;
    }

    fclose(fp);
}

static void init_players(GameState *state_ptr)
{
    init_player(
        &gameplay_state.player_1,
        state_ptr->user1.id,
        gameplay_state.map.player1_pos,
        RIGHT,
        state_ptr->user1_meta);

    init_player(
        &gameplay_state.player_2,
        state_ptr->user2.id,
        gameplay_state.map.player2_pos,
        LEFT,
        state_ptr->user2_meta);
}

static void render_header(GameState *state_ptr, WINDOW *win)
{
    assert(win != NULL);
    wclear(win);

    render_header_game_stats(state_ptr, win);
    render_header_player_stats(state_ptr, win);

    wrefresh(win);
}

static void render_header_game_stats(GameState *state_ptr, WINDOW *win)
{
    // Player 1's won rounds
    wmove(win, 0, 0);
    wattr_on(win, STAR_COLOR_PAIR, NULL);
    for (int i = 0; i < state_ptr->user1_meta.rounds_won; i++)
        wprintw(win, STAR);
    wattr_off(win, STAR_COLOR_PAIR, NULL);

    // Round n of m
    wmove(win, 0, getmaxx(win) / 2 - 6);
    wprintw(win, "Round %d of %d", state_ptr->played_rounds + 1, state_ptr->target_rounds);

    // Player 2's won rounds
    wmove(win, 0, getmaxx(win) - state_ptr->user2_meta.rounds_won);
    wattr_on(win, STAR_COLOR_PAIR, NULL);
    for (int i = 0; i < state_ptr->user2_meta.rounds_won; i++)
        wprintw(win, STAR);
    wattr_off(win, STAR_COLOR_PAIR, NULL);
}

static void render_header_player_stats(GameState *state_ptr, WINDOW *win)
{
    const int max_userinfo_len = getmaxx(win) / 2;
    Player *p1 = &gameplay_state.player_1, *p2 = &gameplay_state.player_2;

    // Player 1's round stats
    char p1_info[max_userinfo_len + 1];
    snprintf(p1_info,
             max_userinfo_len,
             "%s | %d " HEART " | %d " GHOST " | %d " GUN_UPGRADE " | %d " GRENADE,
             state_ptr->user1.username,
             p1->lives,
             p1->remaining_ghost_moves,
             p1->remaining_upgraded_shots,
             p1->remaining_grenades);

    mvwprintw(win, getmaxy(win) - 1, 0, p1_info);

    // Player2's round stats
    char p2_info[max_userinfo_len + 1];
    snprintf(p2_info,
             max_userinfo_len,
             "%d " GRENADE " | %d " GUN_UPGRADE " | %d " GHOST " | %d " HEART " | %s",
             p2->remaining_grenades,
             p2->remaining_upgraded_shots,
             p2->remaining_ghost_moves,
             p2->lives,
             state_ptr->user2.username);

    mvwprintw(win, getmaxy(win) - 1, getmaxx(win) - utf8len(p2_info), p2_info);
}

static void draw_cell(WINDOW *win, Coordinates pos)
{
    AcquirableItem *aitem_ptr = NULL;
    Bullet *bullet_ptr = NULL;

    // Wall
    if (is_wall(&gameplay_state, pos))
        wprintw(win, WALL);
    // Mirror
    else if (is_mirror(&gameplay_state, pos))
        wprintw(win, MIRROR);
    // Blackhole
    else if (is_blackhole(&gameplay_state, pos))
    {
        wattr_on(win, BLACKHOLE_COLOR_PAIR, NULL);
        wprintw(win, BLACKHOLE);
        wattr_off(win, BLACKHOLE_COLOR_PAIR, NULL);
    }
    // Bullet
    else if ((bullet_ptr = get_bullet_by_pos(&gameplay_state, pos)) != NULL)
        draw_bullet(win, bullet_ptr);
    // PLayer 1
    else if (coord_cmp(pos, gameplay_state.player_1.position))
        draw_player(win, &gameplay_state.player_1, PLAYER_1_COLOR_PAIR);
    // PLayer 2
    else if (coord_cmp(pos, gameplay_state.player_2.position))
        draw_player(win, &gameplay_state.player_2, PLAYER_2_COLOR_PAIR);
    // Portal
    else if (is_portal(&gameplay_state, pos))
    {
        wattr_on(win, PORTAL_COLOR_PAIR, NULL);
        wprintw(win, PORTAL);
        wattr_off(win, PORTAL_COLOR_PAIR, NULL);
    }
    // Acquirable items
    else if ((aitem_ptr = get_aitem_by_pos(&gameplay_state, pos)) != NULL)
        draw_aitem(win, aitem_ptr);
    // Empty space
    else
        wprintw(win, SPACE);
}

static void draw_bullet(WINDOW *win, Bullet *bull)
{
    attr_t cp = bull->damage > 1 ? UPGRADED_BULLET_COLOR_PAIR : BULLET_COLOR_PAIR;

    wattr_on(win, cp, NULL);
    wprintw(win, bull->is_grenade
                     ? GRENADE
                 : bull->fired_by_ghost ? GHOST_BULLET
                                        : BULLET);
    wattr_off(win, cp, NULL);
}

static void draw_player(WINDOW *win, Player *player, attr_t color)
{
    char *cell = NULL;

    switch (player->direction)
    {
    case UP:
        cell = PLAYER_UP;
        break;
    case RIGHT:
        cell = PLAYER_RIGHT;
        break;
    case DOWN:
        cell = PLAYER_DOWN;
        break;
    case LEFT:
        cell = PLAYER_LEFT;
        break;
    }

    if (is_ghost(player))
        cell = GHOST;

    wattr_on(win, color, NULL);
    wprintw(win, cell);
    wattr_off(win, color, NULL);
}

static void draw_aitem(WINDOW *win, AcquirableItem *aitem_ptr)
{
    attr_t cp;
    char *cell;

    switch (aitem_ptr->type)
    {
    case AI_FIRST_AID: // First aid
        cell = HEART;
        cp = HEART_COLOR_PAIR;
        break;

    case AI_GRENADE: // Grenade
        cell = GRENADE;
        cp = GRENADE_COLOR_PAIR;
        break;

    case AI_GUN_UPGRADE: // Gn upgrade
        cell = GUN_UPGRADE;
        cp = GUN_UPGRADE_COLOR_PAIR;
        break;

    case AI_GHOST: // Ghost
        cell = GHOST;
        cp = COLOR_PAIR(CP_WHITE_BLACK);
        break;

    default:
        error("Invalid aitem type!");
    }

    wattr_on(win, cp, NULL);
    wprintw(win, cell);
    wattr_off(win, cp, NULL);
}

static FIELD *integer_field(int height, int width, int y, int x, int precision, int min, int max, char *default_value)
{
    FIELD *field = not_null(new_field(height, width, y, x, 0, 0));
    set_field_opts(field, FIELD_OPTS);
    set_field_back(field, FIELDS_BACKGROUND);
    set_field_type(field, TYPE_INTEGER, precision, min, max);
    if (default_value != NULL)
        set_field_buffer(field, 0, default_value);

    return field;
}

static void prompt_game_conf(GameState *state_ptr)
{
    int m_height = 10, m_width = MW_WIDTH / 3;
    int start_x = 2, field_width = m_width - 4;

    FIELD *p1_lives_label = label_field("Enter first player's lives (1-9):", 1, field_width, 1, start_x);
    FIELD *p1_lives_field = integer_field(1, field_width, 2, start_x, 1, 1, 9, "5");

    FIELD *p2_lives_label = label_field("Enter second player's lives (1-9):", 1, field_width, 3, start_x);
    FIELD *p2_lives_field = integer_field(1, field_width, 4, start_x, 1, 1, 9, "5");

    FIELD *total_rounds_label = label_field("Enter total rounds (1-9):", 5, field_width, 5, start_x);
    FIELD *total_rounds_field = integer_field(1, field_width, 6, start_x, 1, 1, 9, "2");

    game_conf_fields[0] = p1_lives_label;
    game_conf_fields[1] = p1_lives_field;
    game_conf_fields[2] = p2_lives_label;
    game_conf_fields[3] = p2_lives_field;
    game_conf_fields[4] = total_rounds_label;
    game_conf_fields[5] = total_rounds_field;
    game_conf_fields[6] = NULL;

    FORM *form = not_null(new_form(game_conf_fields));

    WINDOW *modal_win = init_centered_win(m_height, m_width);
    WINDOW *form_win = not_null(derwin(modal_win, m_height - 2, m_width - 2, 1, 1));

    curs_set(1);

    set_form_win(form, form_win);
    set_form_sub(form, modal_win);
    post_form(form);

    apply_modal_style(modal_win, CP_BLACK_CYAN, "Game configuration");
    wrefresh(form_win);
    wrefresh(modal_win);
    refresh();

    while (!form_handle_keys(form, wgetch(modal_win), NULL))
    {
    }

    curs_set(0);

    int p1_lives = atoi(field_buffer(p1_lives_field, 0)),
        p2_lives = atoi(field_buffer(p2_lives_field, 0)),
        total_rounds = atoi(field_buffer(total_rounds_field, 0));

    state_ptr->user1_meta.initial_lives = p1_lives;
    state_ptr->user2_meta.initial_lives = p2_lives;
    state_ptr->played_rounds = 0;
    state_ptr->target_rounds = total_rounds;

    unpost_form(form);
    destroy_win(form_win);
    destroy_win(modal_win);
}