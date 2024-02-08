#ifndef sw_utils_colors
#define sw_utils_colors

typedef enum
{
    CP_RED_BLACK = 1,
    CP_GREEN_BLACK = 2,
    CP_YELLOW_BLACK = 3,
    CP_BLUE_BLACK = 4,
    CP_MAGENTA_BLACK = 5,
    CP_CYAN_BLACK = 6,
    CP_BLUE_WHITE = 7,
    CP_WHITE_RED = 8,
    CP_BLACK_GREEN = 9,
    CP_BLUE_YELLOW = 10,
    CP_WHITE_BLUE = 11,
    CP_WHITE_MAGENTA = 12,
    CP_BLACK_CYAN = 13,
    CP_WHITE_BLACK = 14,
} ColorPair;

void register_colors(void);

#endif
