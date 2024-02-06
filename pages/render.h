#ifndef sw_pages_render
#define sw_pages_render

#include <ncurses.h>
#include "../game.h"

#define DISALLOW_KEY_HOLD 1

typedef struct
{
    void (*init)(GameState *state_ptr);
    void (*handle_keys)(GameState *state_ptr, int key);
    void (*sync)(GameState *state_ptr);
    void (*render)(WINDOW *win);
    void (*destroy)(GameState *state_ptr);
} PageFuncs;

void tick_page(WINDOW *win, GameState *state_ptr, int key);

#endif