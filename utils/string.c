#include <stdbool.h>
#include <string.h>
#include "string.h"

bool ends_with(char *s1, char *s2)
{
    return strcmp(s1 + strlen(s1) - strlen(s2), s2) == 0;
}
