#ifndef sw_pages_game_finished
#define sw_pages_game_finished

#include <ncurses.h>
#include "render.h"
#include "../utils/colors.h"

#define RESTART_GAME_IN 10
#define DRAW_COLOR_PAIR COLOR_PAIR(CP_CYAN_BLACK)

PageFuncs get_game_finished_funcs(void);

#endif