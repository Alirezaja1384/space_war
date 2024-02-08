#ifndef sw_pages_gameplay_utils
#define sw_pages_gameplay_utils

#include "definations.h"

void init_bullet(Bullet *bullet);
void init_player(Player *player, char *username, Coordinates position, Direction direction, UserMeta meta);

void fire_bullet(GameplayState *gs_ptr, Player *player);
void kill_player(GameplayState *gs_ptr, Player *player, int lost_lives);

void mask_map_pos(GameplayState *gs_ptr, Coordinates pos);

void move_player(Player *player, Direction direction);
void move_bullet(Bullet *bullet);

AcquirableItem *get_aitem_by_pos(GameplayState *gs_ptr, Coordinates pos);
Player *get_player_by_pos(GameplayState *gs_ptr, Coordinates pos);
Bullet *get_bullet_by_pos(GameplayState *gs_ptr, Coordinates pos);

Coordinates get_initial_position(GameplayState *gs_ptr, Player *player);

bool is_wall(GameplayState *gs_ptr, Coordinates pos);
bool is_mirror(GameplayState *gs_ptr, Coordinates pos);
bool is_blackhole(GameplayState *gs_ptr, Coordinates pos);
bool is_portal(GameplayState *gs_ptr, Coordinates pos);
MapObjectType get_map_item(GameplayState *gs_ptr, Coordinates pos);

bool is_ghost(Player *player);
bool has_grenade(Player *player);
bool has_upgraded_gun(Player *player);

bool validate_coord(Coordinates coord);
void move_in_page(Coordinates *pos, Direction dir);

#endif