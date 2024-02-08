#include <ncurses.h>
#include "colors.h"
#include "assertion.h"

#define PAIRS_COUNT 14

void register_colors(void)
{
    check(has_colors() && COLOR_PAIRS >= PAIRS_COUNT, "Color unsupported");

    int pairs[PAIRS_COUNT][3] = {
        {CP_RED_BLACK, COLOR_RED, COLOR_BLACK},
        {CP_GREEN_BLACK, COLOR_GREEN, COLOR_BLACK},
        {CP_YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK},
        {CP_BLUE_BLACK, COLOR_BLUE, COLOR_BLACK},
        {CP_MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK},
        {CP_CYAN_BLACK, COLOR_CYAN, COLOR_BLACK},
        {CP_BLUE_WHITE, COLOR_BLUE, COLOR_WHITE},
        {CP_WHITE_RED, COLOR_WHITE, COLOR_RED},
        {CP_BLACK_GREEN, COLOR_BLACK, COLOR_GREEN},
        {CP_BLUE_YELLOW, COLOR_BLUE, COLOR_YELLOW},
        {CP_WHITE_BLUE, COLOR_WHITE, COLOR_BLUE},
        {CP_WHITE_MAGENTA, COLOR_WHITE, COLOR_MAGENTA},
        {CP_BLACK_CYAN, COLOR_BLACK, COLOR_CYAN},
        {CP_WHITE_BLACK, COLOR_WHITE, COLOR_BLACK},
    };

    for (size_t i = 0; i < PAIRS_COUNT; i++)
    {
        init_pair(pairs[i][0], pairs[i][1], pairs[i][2]);
    }
}
