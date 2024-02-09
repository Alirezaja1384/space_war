#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>
#include <string.h>
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
    static void *map_argvs[MAX_MAPS_COUNT + 1][2];
    static MenuItem menu_items[MAX_MAPS_COUNT + 1];

    int found_count = find_maps(map_paths, MAX_MAPS_COUNT);

    for (int i = 0; i < found_count; i++)
    {
        map_argvs[i][0] = (void *)state_ptr;
        map_argvs[i][1] = (void *)map_paths[i];

        fill_menu_item(menu_items + i, map_paths[i], handle_map_select, map_argvs[i], 2);
    }

    static GameState *back_argv[1];
    back_argv[0] = state_ptr;

    if (state_ptr->played_rounds > 0)
        fill_menu_item(menu_items + found_count, NULL, NULL, NULL, 0);
    else
    {
        // Show main menu item if user didn't play an game yet
        fill_menu_item(
            menu_items + found_count,
            "<|Main menu|>",
            handle_back_to_main_menu,
            (void **)back_argv,
            1);

        fill_menu_item(menu_items + found_count + 1, NULL, NULL, NULL, 0);
    }

    init_menu_state(&sm_menu_state, "Select a map:", menu_items);
}

void select_maps_handle_keys(GameState *state_ptr, int key)
{
    menu_handle_keys(&sm_menu_state, key);
}

void render_select_maps(GameState *state_ptr, WINDOW *win)
{
    wclear(win);
    render_menu(win, &sm_menu_state);
}

void destroy_select_maps(GameState *state_ptr)
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
    assert(argc == 2);
    GameState *state_ptr = argv[0];
    char *map_name = argv[1];

    check(strlen(MAPS_DIR) + strlen(map_name) < 512, "File path too long!");

    char relative_path[512] = MAPS_DIR;
    strcat(relative_path, map_name);

    state_ptr->map_path = realpath(relative_path, NULL);
    state_ptr->page = PAGE_GAMEPLAY;
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