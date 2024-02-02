#ifndef sw_pages_select_map
#define sw_pages_select_map

#define MAX_MAPS_COUNT 10

#define MAPS_DIR "./maps/"
#define MAPS_EXTENSION ".map"

void init_select_maps(GameState *state_ptr);
void select_maps_handle_keys(GameState *state_ptr, int key);
void render_select_maps(WINDOW *win);
void destroy_select_maps(void);

int find_maps(char **file_paths_ptr, int max_finds);

#endif