#include <stdbool.h>
#include <assert.h>

#ifndef sw_utils_assertion
#define sw_utils_assertion

#define not_null(x)         \
    ({                      \
        typeof(x) _x = (x); \
        assert(_x != NULL); \
        _x;                 \
    })

void check(bool cond, char *message);
void error(char *message);

#endif