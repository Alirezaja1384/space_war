#ifndef sw_utils_string
#define sw_utils_string

#include <stdbool.h>

bool ends_with(char *s1, char *s2);
size_t utf8len(char *s);

#endif