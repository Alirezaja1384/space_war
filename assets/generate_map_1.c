#include <assert.h>
#include "../pages/gameplay/definations.h"

Map map;
AcquirableItem a_items[] = {
    {false, AI_FIRST_AID, (Coordinates){5, 74}},
    {false, AI_GUN_UPGRADE, (Coordinates){25, 74}},
    {false, AI_GRENADE, (Coordinates){8, 50}},
    {false, AI_GHOST, (Coordinates){21, 99}},
};
int a_items_count = sizeof(a_items) / sizeof(a_items[0]);

void export_map(char *path);

int main(int argc, char *argv[])
{
    assert(argc == 2);
    char *path = argv[1];

    Coordinates wall_coords[] = {
        {10, 8},
        {10, 9},
        {10, 10},
        {10, 11},
        {10, 12},
        {10, 13},
        {10, 14},

        {20, 8},
        {20, 9},
        {20, 10},
        {20, 11},
        {20, 12},
        {20, 13},
        {20, 14},

        {11, 14},
        {12, 14},
        {13, 14},
        {14, 14},
        {15, 14},
        {16, 14},
        {17, 14},
        {18, 14},
        {19, 14},

        {10, 135},
        {10, 136},
        {10, 137},
        {10, 138},
        {10, 139},
        {10, 140},
        {10, 141},

        {20, 135},
        {20, 136},
        {20, 137},
        {20, 138},
        {20, 139},
        {20, 140},
        {20, 141},

        {11, 135},
        {12, 135},
        {13, 135},
        {14, 135},
        {15, 135},
        {16, 135},
        {17, 135},
        {18, 135},
        {19, 135},

        {4, 27},
        {5, 27},
        {6, 27},

        {23, 27},
        {24, 27},
        {25, 27},

        {4, 27},
        {5, 27},
        {6, 27},

        {23, 27},
        {24, 27},
        {25, 27},

        {4, 122},
        {5, 122},
        {6, 122},

        {23, 122},
        {24, 122},
        {25, 122},

        {19, 46},
        {19, 47},
        {19, 48},
        {19, 49},
        {19, 50},
        {19, 51},
        {19, 52},
        {19, 53},
        {19, 54},
        {19, 55},

        {11, 93},
        {11, 94},
        {11, 95},
        {11, 96},
        {11, 97},
        {11, 98},
        {11, 99},
        {11, 100},
        {11, 101},
        {11, 102},

        {8, 74},
        {9, 74},
        {10, 74},
        {11, 74},
        {12, 74},
        {13, 74},
        {14, 74},
        {16, 74},
        {17, 74},
        {18, 74},
        {19, 74},
        {20, 74},
        {21, 74},
        {22, 74},
    };

    Coordinates mirror_coords[] = {
        {11, 15},
        {12, 15},
        {13, 15},

        {17, 15},
        {18, 15},
        {19, 15},

        {4, 28},
        {5, 28},
        {6, 28},

        {15, 74},

        {23, 121},
        {24, 121},
        {25, 121},

        {11, 134},
        {12, 134},
        {13, 134},

        {17, 134},
        {18, 134},
        {19, 134},
    };

    for (size_t j = 0; j < MAP_HEIGHT; j++)
    {
        for (size_t i = 0; i < MAP_WIDTH; i++)
        {
            if (i == 0 || j == 0 || i == MAP_WIDTH - 1 || j == MAP_HEIGHT - 1)
                map.objects[j][i] = MO_WALL;
            else
                map.objects[j][i] = MO_SPACE;
        }
    }

    for (int i = 0; i < sizeof(wall_coords) / sizeof(wall_coords[0]); i++)
        map.objects[wall_coords[i].y][wall_coords[i].x] = MO_WALL;

    for (int i = 0; i < sizeof(mirror_coords) / sizeof(mirror_coords[0]); i++)
        map.objects[mirror_coords[i].y][mirror_coords[i].x] = MO_MIRROR;

    map.objects[21][35] = MO_BLACKHOLE;
    map.objects[5][130] = MO_BLACKHOLE;

    map.player1_pos = (Coordinates){15, 10};
    map.player2_pos = (Coordinates){15, 139};

    map.portal1_pos = (Coordinates){24, 10};
    map.portal2_pos = (Coordinates){24, 139};

    printf("Exporting map to: %s\n", path);
    export_map(path);
}

bool coord_cmp(Coordinates coord_1, Coordinates coord_2)
{
    return coord_1.x == coord_2.x && coord_1.y == coord_2.y;
}

void export_map(char *path)
{
    FILE *fp = fopen(path, "w");
    fwrite(&map, sizeof(Map), 1, fp);
    fwrite(&a_items, sizeof(AcquirableItem), a_items_count, fp);
    fclose(fp);
}
