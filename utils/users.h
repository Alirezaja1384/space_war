#ifndef sw_utils_users
#define sw_utils_users

#include "../game.h"
#include "../limits.h"

#define USERS_FILE_PATH "./users.txt"
#define MIN_SERIALIZED_USER_LENGTH ID_LENGTH + MIN_USERNAME_LENGTH + 1
#define MAX_SERIALIZED_USER_LENGTH ID_LENGTH + MAX_USERNAME_LENGTH + 2 // {id}:{username}\n

typedef enum
{
    USER_1 = 1,
    USER_2 = 2,
} UserIndex;

typedef struct user_node
{
    User *user;
    struct user_node *next;
} UserNode;

void users_init();
void users_refresh();
void users_free();

void login_user(GameState *state, UserIndex target_user, User *user);
void logout_user(GameState *state, UserIndex target_user);
void apply_user_changes(GameState *state);

UserNode *create_user(const char *username);
bool change_username(User *user, const char *username);

UserNode *get_users_head();
User *get_user_by_id(const char *id);
User *get_user_by_name(const char *name);

#endif