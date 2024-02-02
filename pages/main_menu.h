#ifndef sw_pages_main_menu
#define sw_pages_main_menu

#include <ncurses.h>
#include "../game.h"
#include "render.h"

void init_main_menu(GameState *state_ptr);
void main_menu_handle_keys(GameState *state_ptr, int key);
void render_main_menu(WINDOW *win);
void destroy_main_menu();

PageFuncs get_main_menu_page_funcs(void);

#endif
