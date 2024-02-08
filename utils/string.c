#include <stdbool.h>
#include <string.h>
#include "string.h"

// Checks if s1 end with s2
bool ends_with(char *s1, char *s2)
{
    int s1_len = strlen(s1);
    int s2_len = strlen(s2);
    return s1_len > s2_len && (strcmp(s1 + s1_len - s2_len, s2) == 0);
}

// returns the number of utf8 code points in the buffer at s
size_t utf8len(char *s)
{
    size_t len = 0;
    for (; *s; ++s)
        if ((*s & 0xC0) != 0x80)
            ++len;
    return len;
}