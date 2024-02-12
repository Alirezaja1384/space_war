#include <assert.h>
#include "../pages/gameplay/definations.h"

Map map;
AcquirableItem a_items[] = {
    {false, AI_FIRST_AID, (Coordinates){15, 75}},
    {false, AI_GUN_UPGRADE, (Coordinates){5, 10}},
    {false, AI_GUN_UPGRADE, (Coordinates){24, 139}},
    {false, AI_GRENADE, (Coordinates){15, 62}},
    {false, AI_GRENADE, (Coordinates){15, 88}},
    {false, AI_GHOST, (Coordinates){3, 75}},
    {false, AI_GHOST, (Coordinates){26, 75}},
};
int a_items_count = sizeof(a_items) / sizeof(a_items[0]);

void export_map(char *path);

int main(int argc, char *argv[])
{
    assert(argc == 2);
    char *path = argv[1];

    Coordinates wall_coords[] = {
        {20, 42},
        {20, 41},
        {20, 40},
        {19, 40},
        {19, 39},
        {19, 38},
        {19, 38},
        {18, 38},
        {18, 37},
        {18, 36},
        {17, 36},
        {17, 35},
        {17, 34},
        {16, 34},
        {16, 33},
        {16, 32},

        {15, 30},
        {15, 31},
        {15, 32},

        {14, 32},
        {14, 33},
        {14, 34},
        {13, 34},
        {13, 35},
        {13, 36},
        {12, 36},
        {12, 37},
        {12, 38},
        {11, 38},
        {11, 39},
        {11, 40},
        {10, 40},
        {10, 41},
        {10, 42},

        {5, 60},
        {5, 61},
        {5, 62},
        {5, 63},
        {5, 64},
        {5, 65},
        {5, 66},
        {5, 67},
        {5, 68},
        {5, 69},
        {5, 70},
        {5, 71},
        {5, 72},
        {5, 73},
        {5, 74},
        {5, 75},
        {5, 76},
        {5, 77},
        {5, 78},
        {5, 79},
        {5, 80},
        {5, 81},
        {5, 82},
        {5, 83},
        {5, 84},
        {5, 85},
        {5, 86},
        {5, 87},
        {5, 88},
        {5, 89},
        {5, 90},

        {24, 60},
        {24, 61},
        {24, 62},
        {24, 63},
        {24, 64},
        {24, 65},
        {24, 66},
        {24, 67},
        {24, 68},
        {24, 69},
        {24, 70},
        {24, 71},
        {24, 72},
        {24, 73},
        {24, 74},
        {24, 75},
        {24, 76},
        {24, 77},
        {24, 78},
        {24, 79},
        {24, 80},
        {24, 81},
        {24, 82},
        {24, 83},
        {24, 84},
        {24, 85},
        {24, 86},
        {24, 87},
        {24, 88},
        {24, 89},
        {24, 90},

        {10, 107},
        {10, 108},
        {10, 109},
        {11, 109},
        {11, 110},
        {11, 111},
        {12, 111},
        {12, 112},
        {12, 113},
        {13, 113},
        {13, 114},
        {13, 115},
        {14, 115},
        {14, 116},
        {14, 117},

        {15, 117},
        {15, 118},
        {15, 119},

        {16, 117},
        {16, 116},
        {16, 115},
        {17, 115},
        {17, 114},
        {17, 113},
        {18, 113},
        {18, 112},
        {18, 111},
        {19, 111},
        {19, 110},
        {19, 109},
        {20, 109},
        {20, 108},
        {20, 107},

        {18, 64},
        {18, 65},
        {17, 65},
        {17, 66},
        {16, 66},
        {16, 67},
        {15, 68},
        {15, 67},
        {14, 67},
        {14, 66},
        {13, 66},
        {13, 65},
        {12, 65},
        {12, 64},

        {12, 86},
        {12, 85},
        {13, 85},
        {13, 84},
        {14, 84},
        {14, 83},
        {15, 82},
        {15, 83},
        {16, 83},
        {16, 84},
        {17, 84},
        {17, 85},
        {18, 85},
        {18, 86},
    };

    Coordinates mirror_coords[] = {
        {10, 43},
        {11, 41},
        {12, 39},
        {13, 37},
        {14, 35},
        {15, 33},
        {16, 35},
        {17, 37},
        {18, 39},
        {19, 41},
        {20, 43},

        {12, 63},
        {13, 64},
        {14, 65},
        {15, 66},
        {16, 65},
        {17, 64},
        {18, 63},

        {12, 86},
        {13, 85},
        {14, 84},
        {15, 83},
        {16, 84},
        {17, 85},
        {18, 86},

        {10, 106},
        {11, 108},
        {12, 110},
        {13, 112},
        {14, 114},
        {15, 116},
        {16, 114},
        {17, 112},
        {18, 110},
        {19, 108},
        {20, 106},
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

    // map.objects[13][78] = MO_BLACKHOLE;
    // map.objects[17][72] = MO_BLACKHOLE;
    // map.objects[22][75] = MO_BLACKHOLE;

    map.player1_pos = (Coordinates){15, 10};
    map.player2_pos = (Coordinates){15, 139};

    map.portal1_pos = (Coordinates){24, 10};
    map.portal2_pos = (Coordinates){5, 139};

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
