#include <assert.h>
#include "../pages/gameplay/definations.h"

Map map;
AcquirableItem a_items[] = {
    {false, AI_FIRST_AID, (Coordinates){15, 75}},
    {false, AI_GUN_UPGRADE, (Coordinates){12, 113}},
    {false, AI_GUN_UPGRADE, (Coordinates){12, 36}},
    {false, AI_GRENADE, (Coordinates){23, 75}},
    {false, AI_GHOST, (Coordinates){4, 72}},
};
int a_items_count = sizeof(a_items) / sizeof(a_items[0]);

void export_map(char *path);

int main(int argc, char *argv[])
{
    assert(argc == 2);
    char *path = argv[1];

    Coordinates wall_coords[] = {
        {3, 30},
        {4, 30},
        {5, 30},

        {24, 30},
        {25, 30},
        {26, 30},

        {12, 48},
        {13, 47},
        {14, 46},
        {15, 45},
        {16, 46},
        {17, 47},
        {18, 48},

        {15, 66},
        {15, 67},
        {15, 68},
        {15, 69},
        {15, 70},
        {14, 70},
        {13, 70},
        {12, 70},
        {12, 71},
        {12, 72},
        {12, 73},
        {12, 74},
        {12, 75},
        {11, 75},
        {10, 75},

        {6, 71},
        {6, 72},
        {6, 73},
        {6, 74},
        {6, 75},
        {6, 76},
        {6, 77},

        // ============

        {24, 73},
        {24, 74},
        {24, 75},
        {24, 76},
        {24, 77},
        {24, 78},
        {24, 79},

        {20, 75},
        {19, 75},
        {18, 75},
        {18, 76},
        {18, 77},
        {18, 78},
        {18, 79},
        {18, 80},
        {17, 80},
        {16, 80},
        {15, 80},
        {15, 81},
        {15, 82},
        {15, 83},
        {15, 84},

        {12, 101},
        {13, 102},
        {14, 103},
        {15, 104},
        {16, 103},
        {17, 102},
        {18, 101},

        {3, 119},
        {4, 119},
        {5, 119},

        {24, 119},
        {25, 119},
        {26, 119},
    };

    Coordinates mirror_coords[] = {
        {11, 20},
        {12, 20},
        {13, 20},
        {14, 20},
        {15, 20},
        {16, 20},
        {17, 20},
        {18, 20},
        {19, 20},

        {3, 31},
        {4, 31},

        {24, 31},
        {25, 31},

        {16, 47},

        //==============
        {14, 71},
        {15, 71},

        {14, 102},

        {4, 118},
        {5, 118},

        {25, 118},
        {26, 118},

        {11, 129},
        {12, 129},
        {13, 129},
        {14, 129},
        {15, 129},
        {16, 129},
        {17, 129},
        {18, 129},
        {19, 129},
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

    map.objects[13][78] = MO_BLACKHOLE;
    map.objects[17][72] = MO_BLACKHOLE;
    map.objects[22][75] = MO_BLACKHOLE;

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
