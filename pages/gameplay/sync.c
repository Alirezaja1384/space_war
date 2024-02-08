#include "../../game.h"
#include "definations.h"
#include "utils.h"

static void handle_bullet_clashes(GameplayState *gs_ptr, Bullet *bull_ptr);
static void handle_successful_shot(GameplayState *gs_ptr, Bullet *bull_ptr);

static void handle_player_clashes(GameplayState *gs_ptr, Player *player_ptr);
static void handle_portal_entrance(GameplayState *gs_ptr, Player *player_ptr);
static void handle_blackhole_entrance(GameplayState *gs_ptr, Player *player_ptr);
static void consume_acquirable_items(GameplayState *gs_ptr, Player *player_ptr);

static void finish_round_if_anyone_lost(GameState *game_state_ptr, GameplayState *gameplay_state_ptr);

void sync_gameplay_states(GameState *game_state_ptr, GameplayState *gameplay_state_ptr)
{
    for (
        Bullet *bull_ptr = gameplay_state_ptr->bullets;
        bull_ptr < gameplay_state_ptr->bullets + MAX_ACTIVE_BULLETS;
        bull_ptr++)
    {
        handle_successful_shot(gameplay_state_ptr, bull_ptr);
        handle_bullet_clashes(gameplay_state_ptr, bull_ptr);
    }

    for (int i = 1; i <= 2; i++)
    {
        Player *player_ptr = i == 1 ? &gameplay_state_ptr->player_1 : &gameplay_state_ptr->player_2;

        handle_portal_entrance(gameplay_state_ptr, player_ptr);
        handle_blackhole_entrance(gameplay_state_ptr, player_ptr);
        handle_player_clashes(gameplay_state_ptr, player_ptr);
        consume_acquirable_items(gameplay_state_ptr, player_ptr);
    }

    finish_round_if_anyone_lost(game_state_ptr, gameplay_state_ptr);
}

// When bullet hits the wall
// When bullet reaches a mirror
static void handle_bullet_clashes(GameplayState *gs_ptr, Bullet *bull_ptr)
{
    Coordinates pos = bull_ptr->position;

    // Reset bullet when hits the wall
    if (!(validate_coord(pos)) ||
        (!bull_ptr->fired_by_ghost && // Ghost shots should go through wall
         !bull_ptr->is_grenade &&     // handle_successful_shot() handles the grenade
         is_wall(gs_ptr, pos)))
        init_bullet(bull_ptr);

    // Reverse its direction when reaches a mirror
    if (is_mirror(gs_ptr, pos))
        bull_ptr->direction = reverse_direction(bull_ptr->direction);
}

// Registers the shot when bullet hits a player
static void handle_successful_shot(GameplayState *gs_ptr, Bullet *bull_ptr)
{
    Player *player_ptr = NULL;
    Coordinates bull_pos = bull_ptr->position;

    bool bullet_used = false;

    // Handle normal shot
    if (
        !bull_ptr->is_grenade &&                                    // Shot is not grenade
        (player_ptr = get_player_by_pos(gs_ptr, bull_pos)) != NULL) // Hits a player
    {
        kill_player(gs_ptr, player_ptr, bull_ptr->damage);
        bullet_used = true;
    }
    // Handle grenade shot
    else if (
        bull_ptr->is_grenade &&                          // Shot is a grenade
        (is_wall(gs_ptr, bull_pos) ||                    // Hits a wall
         is_mirror(gs_ptr, bull_pos) ||                  // or hits a mirror
         (get_player_by_pos(gs_ptr, bull_pos) != NULL))) // or hits a player
    {
        int half_range = GRENADE_AFFECT_RANGE / 2;

        Coordinates pos;
        for (pos.y = bull_pos.y - half_range; pos.y <= bull_pos.y + half_range; pos.y++)
        {
            for (pos.x = bull_pos.x - half_range; pos.x <= bull_pos.x + half_range; pos.x++)
            {
                if (is_mirror(gs_ptr, pos))
                    mask_map_pos(gs_ptr, pos);

                if ((player_ptr = get_player_by_pos(gs_ptr, pos)) != NULL)
                    kill_player(gs_ptr, player_ptr, bull_ptr->damage);
            }
        }

        bullet_used = true;
    }

    // Reset bullet after being used
    if (bullet_used)
        init_bullet(bull_ptr);
}

// When player hits a wall or mirror
static void handle_player_clashes(GameplayState *gs_ptr, Player *player_ptr)
{
    // Kill player when something weird happens (Ex: player goes outside the map)
    if (!validate_coord(player_ptr->position))
        kill_player(gs_ptr, player_ptr, 1);

    // Take user back when hits a wall or mirror if it's not a ghost
    if (!is_ghost(player_ptr) && (is_wall(gs_ptr, player_ptr->position) || is_mirror(gs_ptr, player_ptr->position)))
        move_in_page(&player_ptr->position, reverse_direction(player_ptr->direction));
}

// When player enters a portal
static void handle_portal_entrance(GameplayState *gs_ptr, Player *player_ptr)
{
    if (is_portal(gs_ptr, player_ptr->position))
    {
        // Move player to opposite portal
        player_ptr->position =
            coord_cmp(player_ptr->position, gs_ptr->map.portal1_pos)
                ? gs_ptr->map.portal2_pos
                : gs_ptr->map.portal1_pos;

        // Trigger another move to prevent loop
        move_in_page(&player_ptr->position, player_ptr->direction);
    }
}

// When player enters a blackhole
static void handle_blackhole_entrance(GameplayState *gs_ptr, Player *player_ptr)
{
    if (is_blackhole(gs_ptr, player_ptr->position))
        kill_player(gs_ptr, player_ptr, 1);
}

// When user reaches a bomb, first aid, gun upgrade, or ghost
static void consume_acquirable_items(GameplayState *gs_ptr, Player *player_ptr)
{
    AcquirableItem *aitem = get_aitem_by_pos(gs_ptr, player_ptr->position);
    if (aitem == NULL || aitem->is_used)
        return;

    switch (aitem->type)
    {
    case AI_FIRST_AID:
        player_ptr->lives += AI_FIRST_AID_LIVES;
        break;

    case AI_GRENADE:
        player_ptr->remaining_grenades += AI_GRENADE_GRENADES_SHOTS;
        break;

    case AI_GUN_UPGRADE:
        player_ptr->remaining_upgraded_shots += AI_GUN_UPGRADE_UPGRADED_SHOTS;
        break;

    case AI_GHOST:
        player_ptr->remaining_ghost_moves += AI_GHOST_GHOST_MOVES;
        break;
    }

    aitem->is_used = true;
}

static void finish_round_if_anyone_lost(GameState *game_state_ptr, GameplayState *gameplay_state_ptr)
{
    int winner = -1;
    if (gameplay_state_ptr->player_1.lives <= 0)
        winner = 2;
    else if (gameplay_state_ptr->player_2.lives <= 0)
        winner = 1;

    if (winner == -1)
        return;

    game_state_ptr->round_winner = winner;
    game_state_ptr->played_rounds++;

    UserMeta *meta_ptr = winner == 1 ? &game_state_ptr->user1_meta : &game_state_ptr->user2_meta;
    meta_ptr->rounds_won++;

    if (game_state_ptr->played_rounds >= game_state_ptr->target_rounds)
    {
        // TODO: Implement PAGE_GAME_FINISHED
        game_state_ptr->page = PAGE_ROUND_FINISHED;
    }
    else
        game_state_ptr->page = PAGE_ROUND_FINISHED;
}