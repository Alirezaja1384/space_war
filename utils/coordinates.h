#ifndef sw_utils_coordinates
#define sw_utils_coordinates

#include <stdbool.h>

typedef enum
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
} Direction;

typedef struct
{
    int y;
    int x;
} Coordinates;

bool coord_cmp(Coordinates coord_1, Coordinates coord_2);
bool is_coord_valid(Coordinates coord, int height, int width);

Direction reverse_direction(Direction dir);
void move_by_direction(Coordinates *pos, Direction dir);

#endif