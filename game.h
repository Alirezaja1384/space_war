#ifndef sw_game
#define sw_game

#include <stdbool.h>
#include "limits.h"

#define MW_HEIGHT 30
#define MW_WIDTH 150

typedef enum
{
    PAGE_SELECT_USER = 1,
    PAGE_MAIN_MENU,
    PAGE_HISTORY,
    PAGE_SELECT_MAP,
    PAGE_GAMEPLAY,
    PAGE_ROUND_FINISHED,
    PAGE_GAME_FINISHED,
} Page;

typedef struct
{
    char id[ID_LENGTH + 1];
    char username[MAX_USERNAME_LENGTH + 1];
} User;

typedef struct
{
    bool signed_in;
    int initial_lives;
    int rounds_won;
} UserMeta;

typedef struct
{
    Page page;

    User user1;
    User user2;

    UserMeta user1_meta;
    UserMeta user2_meta;

    int round_winner;
    int target_rounds;
    int played_rounds;

    char *map_path;
} GameState;

void init_game(void);
void exit_game(void);

#endif
