#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
#include "assertion.h"
#include "string.h"
#include "random.h"
#include "files.h"
#include "users.h"

UserNode *users_head = NULL;

static UserNode *get_users_tail();

static UserNode *allocate_user_node();
static void free_user_node(UserNode *user_node);

static void serialize_user(User *user, char *buff);
static void deserialize_user(char *buff, User *user);
static void dump_users();

void users_init()
{
    if (users_head == NULL)
        users_refresh();
};

void users_refresh()
{
    users_free();

    UserNode *users_tail = NULL;

    create_file_if_missing(USERS_FILE_PATH);
    FILE *users_file = fopen(USERS_FILE_PATH, "r+");
    check(users_file != NULL, "Unable to open users file");

    while (1)
    {
        char serialized_user[MAX_SERIALIZED_USER_LENGTH + 1];
        fgets(serialized_user, MAX_SERIALIZED_USER_LENGTH, users_file);
        serialized_user[strcspn(serialized_user, "\n")] = '\0';

        if (strlen(serialized_user) < MIN_SERIALIZED_USER_LENGTH)
            break;

        UserNode *user_node = allocate_user_node();
        deserialize_user(serialized_user, user_node->user);

        if (users_head == NULL)
            users_head = users_tail = user_node;
        else
        {
            users_tail->next = user_node;
            users_tail = user_node;
        }

        serialized_user[0] = '\0';
    }
};

void users_free()
{
    UserNode *next_user_node = NULL;
    UserNode *current_node = users_head;

    while (current_node != NULL)
    {
        next_user_node = current_node->next;
        free_user_node(current_node);
        current_node = next_user_node;
    }

    users_head = NULL;
}

void login_user(GameState *state, UserIndex target_user, User *user)
{
    User *state_user_ptr = target_user == USER_1 ? &state->user1 : &state->user2;
    UserMeta *meta_ptr = target_user == USER_1 ? &state->user1_meta : &state->user2_meta;

    memcpy(state_user_ptr, user, sizeof(User));
    meta_ptr->signed_in = true;
}

void logout_user(GameState *state, UserIndex target_user)
{
    UserMeta *meta_ptr = target_user == USER_1 ? &state->user1_meta : &state->user2_meta;
    meta_ptr->signed_in = false;
}

void apply_user_changes(GameState *state)
{
    dump_users();

    // Login users again to update their profiles
    if (state->user1_meta.signed_in)
        login_user(state, USER_1, get_user_by_id(state->user1.id));

    if (state->user2_meta.signed_in)
        login_user(state, USER_2, get_user_by_id(state->user2.id));
}

UserNode *create_user(const char *username)
{
    UserNode *unode = allocate_user_node();

    if (get_user_by_name(username) != NULL)
        return NULL;

    // Set id
    generate_uuid(unode->user->id);
    // Set username
    unode->user->username[0] = '\0';
    strncat(unode->user->username, username, MAX_USERNAME_LENGTH);

    UserNode *tail = get_users_tail();
    if (tail == NULL)
        users_head = tail = unode;
    else
        tail->next = unode;

    dump_users();
}

bool change_username(User *user, const char *username)
{
    if (get_user_by_name(username) != NULL)
        return false;

    user->username[0] = '\0';
    strncat(user->username, username, MAX_USERNAME_LENGTH);
    return true;
}

UserNode *get_users_head()
{
    users_init();
    return users_head;
}

static UserNode *get_users_tail()
{
    UserNode *node = get_users_head();
    while (node != NULL && node->next != NULL)
    {
        node = node->next;
    }

    return node;
}

User *get_user_by_id(const char *id)
{
    UserNode *current_node = get_users_head();
    while (current_node != NULL)
    {
        if (strcmp(current_node->user->id, id) == 0)
            return current_node->user;

        current_node = current_node->next;
    }

    return NULL;
}

User *get_user_by_name(const char *name)
{
    UserNode *current_node = get_users_head();
    while (current_node != NULL)
    {
        if (strcmp(current_node->user->username, name) == 0)
            return current_node->user;

        current_node = current_node->next;
    }

    return NULL;
}

static UserNode *allocate_user_node()
{
    User *user = malloc(sizeof(User));
    UserNode *user_node = malloc(sizeof(UserNode));
    check(user != NULL && user_node != NULL, "Unable to allocate user node");

    user_node->user = user;
    return user_node;
}

static void free_user_node(UserNode *user_node)
{
    free(user_node->user);
    free(user_node);
}

static void serialize_user(User *user, char *buff)
{
    snprintf(buff, MAX_SERIALIZED_USER_LENGTH, "%s:%s", user->id, user->username);
}

static void deserialize_user(char *buff, User *user)
{
    int buff_len = strlen(buff);
    assert(buff_len >= MIN_SERIALIZED_USER_LENGTH);

    buff += copy_until_delimiter(user->id, buff, ':', ID_LENGTH) + 1;
    copy_until_delimiter(user->username, buff, ':', MAX_USERNAME_LENGTH);
}

static void dump_users()
{
    FILE *users_file = fopen(USERS_FILE_PATH, "w");
    check(users_file != NULL, "Unable to open users file");

    UserNode *current_node = get_users_head();
    while (current_node != NULL)
    {
        char serialized_user[MAX_SERIALIZED_USER_LENGTH + 1];
        serialize_user(current_node->user, serialized_user);
        fprintf(users_file, "%s\n", serialized_user);

        current_node = current_node->next;
    }

    fclose(users_file);
}