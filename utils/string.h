#ifndef sw_utils_string
#define sw_utils_string

#include <stdbool.h>
#include <stdlib.h>

bool ends_with(char *s1, char *s2);
size_t utf8len(char *s);
char *trim_whitespaces(char *str);
int copy_until_delimiter(char *s1, char *s2, char delimiter, int max);

#endif