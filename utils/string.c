#include <stdbool.h>
#include <string.h>
#include "string.h"

bool ends_with(char *s1, char *s2)
{
    int s1_len = strlen(s1);
    int s2_len = strlen(s2);
    return s1_len > s2_len && (strcmp(s1 + s1_len - s2_len, s2) == 0);
}
