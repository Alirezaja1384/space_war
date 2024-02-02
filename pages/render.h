#ifndef sw_pages_render
#define sw_pages_render

#include <ncurses.h>
#include "../game.h"

void tick_page(WINDOW *win, GameState *state_ptr, int key);

#endif