#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include "../utils/assertion.h"
#include "../utils/string.h"
#include "../utils/menu.h"
#include "../game.h"
#include "select_map.h"

MenuState sm_menu_state;
char *map_paths[MAX_MAPS_COUNT];

int find_maps(char **file_paths_ptr, int max_finds);

void handle_map_select(void *argv[], int argc);
void handle_back_to_main_menu(void *argv[], int argc);

void init_select_maps(GameState *state_ptr)
{
    static char *map_argvs[MAX_MAPS_COUNT + 1][1];

    static MenuItem menu_items[MAX_MAPS_COUNT + 1];

    int found_count = find_maps(map_paths, MAX_MAPS_COUNT);

    for (int i = 0; i < found_count; i++)
    {
        map_argvs[i][0] = map_paths[i];
        fill_menu_item(menu_items + i, map_paths[i], handle_map_select, (void **)map_argvs[i], 1);
    }

    static GameState *back_argv[1];
    back_argv[0] = state_ptr;

    fill_menu_item(
        menu_items + found_count,
        "<|Main menu|>",
        handle_back_to_main_menu,
        (void **)back_argv,
        1);

    fill_menu_item(menu_items + found_count + 1, NULL, NULL, NULL, 0);

    init_menu_state(&sm_menu_state, menu_items);
}

void select_maps_handle_keys(GameState *state_ptr, int key)
{
    menu_handle_keys(&sm_menu_state, key);
}

void render_select_maps(WINDOW *win)
{
    render_menu(win, &sm_menu_state);
}

void destroy_select_maps(void)
{
    sm_menu_state.current_item = 0;
}

int find_maps(char **file_paths, int max_finds)
{
    // Go to directory
    DIR *maps_dir = opendir(MAPS_DIR);
    check(maps_dir != NULL, "Unable to open maps folder!");

    int found_count = 0;

    // Loop over dir entries
    struct dirent *ent;
    while ((ent = readdir(maps_dir)) != NULL && found_count < max_finds)
    {
        // Add map files to file_paths and increase found_count by one
        if (ends_with(ent->d_name, MAPS_EXTENSION))
            file_paths[found_count++] = ent->d_name;
    }

    closedir(maps_dir);
    return found_count;
}

void handle_back_to_main_menu(void *argv[], int argc)
{
    assert(argc == 1);
    GameState *state_ptr = argv[0];
    state_ptr->page = PAGE_MAIN_MENU;
}

void handle_map_select(void *argv[], int argc)
{
    assert(argc == 1);
    char *map_path = argv[0];
}

PageFuncs get_select_map_page_funcs(void)
{
    PageFuncs funcs = {
        init_select_maps,
        select_maps_handle_keys,
        NULL,
        render_select_maps,
        destroy_select_maps};

    return funcs;
}