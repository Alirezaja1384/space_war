#include <stdbool.h>
#include <ncurses.h>
#include "coordinates.h"

bool coord_cmp(Coordinates coord_1, Coordinates coord_2)
{
    return coord_1.x == coord_2.x && coord_1.y == coord_2.y;
}

bool is_coord_valid(Coordinates coord, int height, int width)
{
    return coord.x >= 0 && coord.x < width &&
           coord.y >= 0 && coord.y < height;
}

Direction reverse_direction(Direction dir)
{
    switch (dir)
    {
    case UP:
        return DOWN;
        break;

    case RIGHT:
        return LEFT;
        break;

    case DOWN:
        return UP;
        break;
    case LEFT:
        return RIGHT;
        break;
    }
}

void move_by_direction(Coordinates *pos, Direction dir)
{
    switch (dir)
    {
    case UP:
        pos->y--;
        break;

    case RIGHT:
        pos->x++;
        break;

    case DOWN:
        pos->y++;
        break;

    case LEFT:
        pos->x--;
        break;
    }
}
