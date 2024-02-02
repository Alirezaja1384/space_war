#include <stdbool.h>

#ifndef sw_utils_assertion
#define sw_utils_assertion

void check(bool cond, char *message);
void error(char *message);

#endif