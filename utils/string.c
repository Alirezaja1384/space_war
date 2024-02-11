#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "string.h"

// Checks if s1 end with s2
bool ends_with(char *s1, char *s2)
{
    int s1_len = strlen(s1);
    int s2_len = strlen(s2);
    return s1_len > s2_len && (strcmp(s1 + s1_len - s2_len, s2) == 0);
}

// Returns the number of utf8 code points in the buffer at s
size_t utf8len(char *s)
{
    size_t len = 0;
    for (; *s; ++s)
        if ((*s & 0xC0) != 0x80)
            ++len;
    return len;
}

// Removes whitespaces from the start and end of a string
char *trim_whitespaces(char *str)
{
    char *end;

    // trim leading space
    while (isspace(*str))
        str++;

    if (*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while (end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end + 1) = '\0';

    return str;
}

int copy_until_delimiter(char *s1, char *s2, char delimiter, int max)
{
    int copied_chars = 0;
    while (copied_chars < max && *s2 != delimiter && s2 != "\0")
    {
        *s1 = *s2;
        s1++;
        s2++;
        copied_chars++;
    }
    s1[copied_chars] = '\0';

    return copied_chars;
}
