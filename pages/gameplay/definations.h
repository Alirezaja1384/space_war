#ifndef sw_pages_gameplay_types
#define sw_pages_gameplay_types

#include "../../utils/coordinates.h"
#include "../../utils/colors.h"
#include "../../game.h"
#include "../../limits.h"
#include "../render.h"

#define MAP_HEIGHT MW_HEIGHT
#define MAP_WIDTH MW_WIDTH

#define BULLET_MOVE_INTERVAL_MILISECONDS 25

#define MAX_ACQUIRABLE_ITEMS 15
#define MAX_ACTIVE_BULLETS 20

#define UPGRADED_GUN_MULTIPLIER 2
#define GRENADE_AFFECT_RANGE 5

#define AI_FIRST_AID_LIVES 5
#define AI_GRENADE_GRENADES_SHOTS 1
#define AI_GUN_UPGRADE_UPGRADED_SHOTS 5
#define AI_GHOST_GHOST_MOVES 7

#define K_PLAYER_1_UP 'w'
#define K_PLAYER_1_RIGHT 'd'
#define K_PLAYER_1_DOWN 's'
#define K_PLAYER_1_LEFT 'a'
#define K_PLAYER_1_SHOOT 'c'

#define K_PLAYER_2_UP 'i'
#define K_PLAYER_2_RIGHT 'l'
#define K_PLAYER_2_DOWN 'k'
#define K_PLAYER_2_LEFT 'j'
#define K_PLAYER_2_SHOOT 'n'

#define PLAYER_UP "⮝"
#define PLAYER_RIGHT "⮞"
#define PLAYER_DOWN "⮟"
#define PLAYER_LEFT "⮜"
#define GHOST "⍝"

#define PLAYER_1_COLOR_PAIR COLOR_PAIR(CP_GREEN_BLACK)
#define PLAYER_2_COLOR_PAIR COLOR_PAIR(CP_RED_BLACK)

#define BULLET "•"
#define GHOST_BULLET "-"
#define GRENADE "Ơ"

#define BULLET_COLOR_PAIR COLOR_PAIR(CP_YELLOW_BLACK)
#define GRENADE_COLOR_PAIR COLOR_PAIR(CP_MAGENTA_BLACK);
#define UPGRADED_BULLET_COLOR_PAIR COLOR_PAIR(CP_MAGENTA_BLACK)

#define STAR "★"
#define STAR_COLOR_PAIR COLOR_PAIR(CP_YELLOW_BLACK)

#define HEART "♥"
#define HEART_COLOR_PAIR COLOR_PAIR(CP_RED_BLACK)

#define GUN_UPGRADE "🗲"
#define GUN_UPGRADE_COLOR_PAIR COLOR_PAIR(CP_CYAN_BLACK)

#define BLACKHOLE "෧"
#define BLACKHOLE_COLOR_PAIR COLOR_PAIR(CP_BLUE_BLACK)

#define PORTAL "֍"
#define PORTAL_COLOR_PAIR COLOR_PAIR(CP_CYAN_BLACK)

#define WALL "█"
#define SPACE " "
#define MIRROR "‖"

typedef enum
{
    MO_SPACE,
    MO_WALL,
    MO_MIRROR,
    MO_BLACKHOLE,
} MapObjectType;

typedef struct
{
    Coordinates player1_pos;
    Coordinates player2_pos;

    Coordinates portal1_pos;
    Coordinates portal2_pos;

    MapObjectType objects[MAP_HEIGHT][MAP_WIDTH];
} Map;

typedef enum
{
    AI_FIRST_AID,
    AI_GRENADE,
    AI_GUN_UPGRADE,
    AI_GHOST,
} AcquirableItemType;

typedef struct
{
    bool is_used;
    AcquirableItemType type;
    Coordinates position;
} AcquirableItem;

typedef struct
{
    char id[ID_LENGTH + 1];

    int lives;

    int remaining_ghost_moves;
    int remaining_upgraded_shots;
    int remaining_grenades;

    Coordinates position;
    Direction direction;
} Player;

typedef struct
{
    bool is_grenade;
    bool fired_by_ghost;

    int damage;

    Coordinates position;
    Direction direction;
} Bullet;

typedef struct
{
    bool sync_needed;
    bool render_needed;

    Player player_1;
    Player player_2;

    Bullet bullets[MAX_ACTIVE_BULLETS];

    Map map;
    bool map_mask[MAP_HEIGHT][MAP_WIDTH];

    size_t aitems_count;
    AcquirableItem aitems[MAX_ACQUIRABLE_ITEMS];
} GameplayState;

#endif