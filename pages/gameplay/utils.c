#include <string.h>
#include "../../utils/assertion.h"
#include "../../utils/coordinates.h"
#include "definations.h"
#include "utils.h"

void init_player(Player *player_ptr, char *id, Coordinates position, Direction direction, UserMeta meta)
{
    player_ptr->id[0] = '\0';
    strncat(player_ptr->id, id, ID_LENGTH);

    player_ptr->lives = meta.initial_lives;

    player_ptr->position = position;
    player_ptr->direction = direction;

    player_ptr->remaining_ghost_moves = 0;
    player_ptr->remaining_grenades = 0;
    player_ptr->remaining_upgraded_shots = 0;
}

void init_bullet(Bullet *bullet)
{
    bullet->position = (Coordinates){-1, -1};
    bullet->damage = 1;
    bullet->fired_by_ghost = false;
    bullet->is_grenade = false;
}

void fire_bullet(GameplayState *gs_ptr, Player *player_ptr)
{
    for (int i = 0; i < MAX_ACTIVE_BULLETS; i++)
    {
        Bullet *bull = gs_ptr->bullets + i;
        if (!validate_coord(bull->position))
        {
            init_bullet(bull); // Reset bullet

            bull->direction = player_ptr->direction;
            bull->position = player_ptr->position;
            move_in_page(&bull->position, player_ptr->direction);

            if (has_grenade(player_ptr))
            {
                bull->is_grenade = true;
                player_ptr->remaining_grenades--;
            }

            // Being ghost should not affect grenade
            if (!bull->is_grenade && is_ghost(player_ptr))
            {
                bull->fired_by_ghost = true;
                player_ptr->remaining_ghost_moves--;
            }

            if (has_upgraded_gun(player_ptr))
            {
                bull->damage *= UPGRADED_GUN_MULTIPLIER;
                player_ptr->remaining_upgraded_shots--;
            }

            break;
        }
    }
}

void kill_player(GameplayState *gs_ptr, Player *player_ptr, int lost_lives)
{
    player_ptr->remaining_ghost_moves = 0;
    player_ptr->remaining_grenades = 0;
    player_ptr->remaining_upgraded_shots = 0;
    player_ptr->lives = lost_lives > player_ptr->lives ? 0 : player_ptr->lives - lost_lives;

    player_ptr->position = get_initial_position(gs_ptr, player_ptr);
}

void mask_map_pos(GameplayState *gs_ptr, Coordinates pos)
{
    gs_ptr->map_mask[pos.y][pos.x] = 0;
}

void move_player(Player *player_ptr, Direction direction)
{
    if (player_ptr->direction != direction)
        player_ptr->direction = direction;

    if (is_ghost(player_ptr))
        player_ptr->remaining_ghost_moves--;

    move_in_page(&player_ptr->position, direction);
}

void move_bullet(Bullet *bullet)
{
    // NOTE: move_in_page() can't be used here because bullets need
    //      to end up in an invalid position to be reused.
    if (validate_coord(bullet->position))
        move_by_direction(&bullet->position, bullet->direction);
    else
        bullet->position = (Coordinates){-1, -1};
}

AcquirableItem *get_aitem_by_pos(GameplayState *gs_ptr, Coordinates pos)
{
    for (int i = 0; i < gs_ptr->aitems_count; i++)
    {
        AcquirableItem *aitem = gs_ptr->aitems + i;
        if (!aitem->is_used && coord_cmp(pos, aitem->position))
            return aitem;
    }

    return NULL;
}

Player *get_player_by_pos(GameplayState *gs_ptr, Coordinates pos)
{
    if (coord_cmp(pos, gs_ptr->player_1.position))
        return &gs_ptr->player_1;
    else if (coord_cmp(pos, gs_ptr->player_2.position))
        return &gs_ptr->player_2;

    return NULL;
}

Bullet *get_bullet_by_pos(GameplayState *gs_ptr, Coordinates pos)
{
    for (int i = 0; i < MAX_ACTIVE_BULLETS; i++)
        if (coord_cmp(pos, gs_ptr->bullets[i].position))
            return gs_ptr->bullets + i;

    return NULL;
}

Coordinates get_initial_position(GameplayState *gs_ptr, Player *player_ptr)
{
    if (player_ptr == &gs_ptr->player_1)
        return gs_ptr->map.player1_pos;
    else if (player_ptr == &gs_ptr->player_2)
        return gs_ptr->map.player2_pos;
    else
        error("Invalid player pointer!");
}

bool is_wall(GameplayState *gs_ptr, Coordinates pos)
{
    return get_map_item(gs_ptr, pos) == MO_WALL;
}

bool is_mirror(GameplayState *gs_ptr, Coordinates pos)
{
    return get_map_item(gs_ptr, pos) == MO_MIRROR;
}

bool is_blackhole(GameplayState *gs_ptr, Coordinates pos)
{
    return get_map_item(gs_ptr, pos) == MO_BLACKHOLE;
}

bool is_portal(GameplayState *gs_ptr, Coordinates pos)
{
    return coord_cmp(pos, gs_ptr->map.portal1_pos) ||
           coord_cmp(pos, gs_ptr->map.portal2_pos);
}

MapObjectType get_map_item(GameplayState *gs_ptr, Coordinates pos)
{
    // Mask item if it's destroyed
    if (!gs_ptr->map_mask[pos.y][pos.x])
        return MO_SPACE;

    return gs_ptr->map.objects[pos.y][pos.x];
}

bool validate_coord(Coordinates coord)
{
    return is_coord_valid(coord, MAP_HEIGHT, MAP_WIDTH);
}

bool is_ghost(Player *player_ptr)
{
    return player_ptr->remaining_ghost_moves > 0;
}

bool has_grenade(Player *player_ptr)
{
    return player_ptr->remaining_grenades > 0;
}

bool has_upgraded_gun(Player *player_ptr)
{
    return player_ptr->remaining_upgraded_shots > 0;
}

void move_in_page(Coordinates *pos, Direction dir)
{
    Coordinates new_pos = *pos;
    move_by_direction(&new_pos, dir);

    if (validate_coord(new_pos))
        *pos = new_pos;
}