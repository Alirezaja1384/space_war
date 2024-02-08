#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../../utils/tui.h"
#include "../../utils/string.h"
#include "../../utils/assertion.h"
#include "../../utils/coordinates.h"
#include "../render.h"
#include "definations.h"
#include "utils.h"
#include "sync.h"
#include "page.h"

WINDOW *header_win = NULL;
GameplayState gameplay_state;

static void load_map(char *map_path);
static void init_players(GameState *state_ptr);

static void render_header(GameState *state_ptr, WINDOW *win);
void render_header_game_stats(GameState *state_ptr, WINDOW *win);
void render_header_player_stats(GameState *state_ptr, WINDOW *win);

static void draw_cell(WINDOW *win, Coordinates pos);

static void draw_bullet(WINDOW *win, Bullet *bull);
static void draw_player(WINDOW *win, Player *player, attr_t color);
static void draw_aitem(WINDOW *win, AcquirableItem *aitem_ptr);

static void init_gameplay(GameState *state_ptr)
{
    assert(state_ptr->map_path != NULL);
    assert(state_ptr->user1.username != NULL);
    assert(state_ptr->user2.username != NULL);

    // Initialize map and acquirable items
    gameplay_state.aitems_count = 0;
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
        state_ptr->user1.username,
        gameplay_state.map.player1_pos,
        RIGHT,
        state_ptr->user1_meta);

    init_player(
        &gameplay_state.player_2,
        state_ptr->user2.username,
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

void render_header_game_stats(GameState *state_ptr, WINDOW *win)
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

void render_header_player_stats(GameState *state_ptr, WINDOW *win)
{
    const int max_userinfo_len = getmaxx(win) / 2;
    Player *p1 = &gameplay_state.player_1, *p2 = &gameplay_state.player_2;

    // Player 1's round stats
    char p1_info[max_userinfo_len + 1];
    snprintf(p1_info,
             max_userinfo_len,
             "%s | %d " HEART " | %d " GHOST " | %d " GUN_UPGRADE " | %d " GRENADE,
             p1->username,
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
             p2->username);

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
