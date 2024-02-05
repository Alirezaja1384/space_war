#ifndef sw_game
#define sw_game

#define MW_HEIGHT 30
#define MW_WIDTH 150

typedef enum
{
    PAGE_MAIN_MENU = 1,
    PAGE_SELECT_MAP = 2,
} Page;

typedef struct
{
    Page page;
} GameState;

void init_game(void);

void exit_game(void);

#endif
