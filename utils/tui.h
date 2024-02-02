#ifndef sw_utils_tui
#define sw_utils_tui

#include <ncurses.h>

#define MW_HEIGHT 30
#define MW_WIDTH 150

#define Y_START (LINES - MW_HEIGHT) / 2
#define X_START (COLS - MW_WIDTH) / 2

WINDOW *setup_tui(void);
void destroy_tui(void);

WINDOW *init_mainwin(void);
WINDOW *get_mainwin(void);

WINDOW *get_win(void);

#endif
