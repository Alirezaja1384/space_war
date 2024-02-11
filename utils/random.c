#include <stdio.h>
#include "assertion.h"
#include "random.h"

void generate_uuid(char *buff)
{
    FILE *fp = not_null(fopen("/proc/sys/kernel/random/uuid", "r"));
    fscanf(fp, "%36s", buff);
    fclose(fp);
}
