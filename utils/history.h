#ifndef sw_utils_history
#define sw_utils_history

#include "../game.h"
#include "../limits.h"

#define HIST_FILE_PATH "./history.hist"

typedef enum
{
    DRAW,
    PLAYER_1_WON,
    PLAYER_2_WON
} GameResult;

typedef struct
{
    char player_1_id[ID_LENGTH + 1];
    char player_2_id[ID_LENGTH + 1];
    GameResult result;
} HistRecord;

typedef struct
{
    HistRecord **records;
    int count;
} HistFetchResult;

HistFetchResult *get_related_hist_records(const char *user_id, int n);
void append_hist_record(HistRecord record);

void free_hist_fetch_result(HistFetchResult *fetch_result);

#endif
