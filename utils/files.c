#include <stdio.h>
#include <stdlib.h>
#include "assertion.h"
#include "files.h"

void create_file_if_missing(const char *path)
{
    FILE *fp = fopen(path, "a");
    check(fp != NULL, "Unable to create file!");

    fclose(fp);
}
