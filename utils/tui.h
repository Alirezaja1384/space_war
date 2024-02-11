#ifndef sw_utils_tui
#define sw_utils_tui

#include <ncurses.h>

#define LOCALE "en_US.utf8"
#define NCURSES_TIMEOUT 50

WINDOW *setup_tui(int height, int width);
void destroy_tui(void);

WINDOW *init_centered_win(int height, int width);
WINDOW *init_header_win(int mainwin_height, int mainwin_width);
void destroy_win(WINDOW *win);

WINDOW *get_win(void);

#endif
