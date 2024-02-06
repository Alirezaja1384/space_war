#ifndef sw_game
#define sw_game

#include "limits.h"

#define MW_HEIGHT 30
#define MW_WIDTH 150

typedef enum
{
    PAGE_MAIN_MENU = 1,
    PAGE_SELECT_MAP = 2,
    PAGE_GAMEPLAY = 3,
} Page;

typedef struct
{
    char username[MAX_USERNAME_LENGTH + 1];
    char email[MAX_EMAIL_LENGTH + 1];
    char password[MAX_PASSWORD_LENGTH + 1];
} User;

typedef struct
{
    int initial_lives;
    int current_game_scores;
} UserMeta;

typedef struct
{
    Page page;

    User user1;
    User user2;

    UserMeta user1_meta;
    UserMeta user2_meta;

    char *map_path;
} GameState;

void init_game(void);
void exit_game(void);

#endif
