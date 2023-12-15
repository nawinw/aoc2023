#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

int64_t main(int64_t argc, char* argv[])
{
    uint64_t seeds[64];
    size_t count = 0;

    FILE *f = fopen(argv[1], "r");
    char buf[1024];
    fgets(buf, sizeof(buf), f);
    printf("%s", buf);
    char *tok = strtok(buf, " ");
    while(tok)
    {
        if (isdigit(tok[0]))
        {
            seeds[count] = atoi(tok);
            count++;
        }
        tok = strtok(NULL, " ");
    }


    int updated[64];
    memset(updated, 0, sizeof(updated));
    while (!feof(f))
    {
        fgets(buf, sizeof(buf), f);
        if (isdigit(buf[0]))
        {
            uint64_t dst_start, src_start, length;
            sscanf(buf, "%lu %lu %lu", &dst_start, &src_start, &length);
            //printf("%lu %lu %lu\n", dst_start, src_start, length);
            for (int i = 0; i < count; i++)
            {
                if (updated[i])
                    continue;

                if (seeds[i] >= src_start && seeds[i] < src_start + length)
                {
                    seeds[i] = dst_start + seeds[i] - src_start;
                    updated[i] = 1;
                }
            }
        }
        else if (isalpha(buf[0]))
        {
            memset(updated, 0, sizeof(updated));
        }
    }
    uint64_t lowest = seeds[0];
    for (int i = 1; i < count; i++)
    {
        if (seeds[i] < lowest)
            lowest = seeds[i];
    }
    printf("%lu\n", lowest);
}

