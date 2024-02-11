#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../game.h"
#include "assertion.h"
#include "files.h"
#include "history.h"

HistFetchResult *get_related_hist_records(const char *user_id, int n)
{
    HistFetchResult *fetch_result = not_null(malloc(sizeof(HistFetchResult)));
    fetch_result->records = (HistRecord **)not_null(malloc(sizeof(HistRecord *) * n));

    create_file_if_missing(HIST_FILE_PATH);
    FILE *fp = not_null(fopen(HIST_FILE_PATH, "r"));

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    int hist_record_size = sizeof(HistRecord);

    size_t cur = file_size;
    HistRecord *record = NULL;
    while (cur >= hist_record_size && fetch_result->count < n)
    {
        cur -= hist_record_size;

        if (record == NULL)
            record = (HistRecord *)malloc(sizeof(HistRecord));

        fseek(fp, cur, SEEK_SET);
        assert(fread(record, hist_record_size, 1, fp) == 1);

        if (strcmp(record->player_1_id, user_id) == 0 || strcmp(record->player_2_id, user_id) == 0)
        {
            fetch_result->records[fetch_result->count] = record;
            fetch_result->count++;
            record = NULL;
        }
    }

    free(record);
    fclose(fp);

    return fetch_result;
}

void append_hist_record(HistRecord record)
{
    FILE *fp = not_null(fopen(HIST_FILE_PATH, "a"));
    fwrite(&record, sizeof(record), 1, fp);
    fclose(fp);
}

void free_hist_fetch_result(HistFetchResult *fetch_result)
{
    for (int i = 0; i < fetch_result->count; i++)
        free(fetch_result->records[i]);

    free(fetch_result->records);
    free(fetch_result);
}