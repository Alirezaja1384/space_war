#ifndef sw_limits
#define sw_limits

#define ID_LENGTH 36
#define ID_LENGTH_STR "36"

#define MIN_USERNAME_LENGTH 3
#define MIN_USERNAME_LENGTH_STR "3"
#define MAX_USERNAME_LENGTH 50
#define MAX_USERNAME_LENGTH_STR "50"

#define USERNAME_REGEX "^[a-zA-Z0-9\\-\\_]{" MIN_USERNAME_LENGTH_STR "," MAX_USERNAME_LENGTH_STR "} *$"

#endif
