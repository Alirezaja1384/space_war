#include <assert.h>
#include <stdlib.h>
#include "../pages/gameplay/definations.h"

Map map;
size_t a_items_count = 0;
AcquirableItem a_items[MAX_ACQUIRABLE_ITEMS];

void load_map(char *path);
void print_map();
bool coord_cmp(Coordinates coord_1, Coordinates coord_2);

int main(int argc, char **argv)
{
    assert(argc == 2);
    load_map(argv[1]);
    print_map();
}

AcquirableItem *get_aitem_by_coords(Coordinates coords)
{
    for (size_t i = 0; i < a_items_count; i++)
    {
        if (coord_cmp(coords, a_items[i].position))
            return a_items + i;
    }
    return NULL;
}

void load_map(char *path)
{
    FILE *fp = fopen(path, "r");
    fread(&map, sizeof(Map), 1, fp);

    while (a_items_count < MAX_ACQUIRABLE_ITEMS)
    {
        if (fread(a_items + a_items_count, sizeof(AcquirableItem), 1, fp) == 1)
            a_items_count++;
        else
            break;
    }

    fclose(fp);
}

void print_map()
{
    for (size_t j = 0; j < MAP_HEIGHT; j++)
    {
        for (size_t i = 0; i < MAP_WIDTH; i++)
        {

            switch (map.objects[j][i])
            {
            case MO_SPACE:
                Coordinates coords = (Coordinates){j, i};
                AcquirableItem *a_item_ptr;
                if (coord_cmp(coords, map.player1_pos))
                    printf(PLAYER_RIGHT);
                else if (coord_cmp(coords, map.player2_pos))
                    printf(PLAYER_LEFT);
                else if (coord_cmp(coords, map.portal1_pos))
                    printf(PORTAL);
                else if (coord_cmp(coords, map.portal2_pos))
                    printf(PORTAL);
                else if ((a_item_ptr = get_aitem_by_coords(coords)) != NULL)
                {
                    switch (a_item_ptr->type)
                    {
                    case AI_FIRST_AID:
                        printf(HEART);
                        break;
                    case AI_GRENADE:
                        printf(GRENADE);
                        break;
                    case AI_GUN_UPGRADE:
                        printf(GUN_UPGRADE);
                        break;
                    case AI_GHOST:
                        printf(GHOST);
                        break;
                    default:
                        printf("?");
                        break;
                    }
                }
                else
                {
                    printf(SPACE);
                }
                break;
            case MO_WALL:
                printf(WALL);
                break;
            case MO_MIRROR:
                printf(MIRROR);
                break;
            case MO_BLACKHOLE:
                printf(BLACKHOLE);
                break;
            default:
                printf("?");
                break;
            }
        }

        printf("\n");
    }
}

bool coord_cmp(Coordinates coord_1, Coordinates coord_2)
{
    return coord_1.x == coord_2.x && coord_1.y == coord_2.y;
}