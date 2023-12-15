#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

int valid(char *str, int *record, int record_len)
{
    int count = 0;
    int counting = 0;
    int record_idx = 0;
    while(1)
    {
        if (!counting && *str == '#')
        {
            count++;
            counting = 1;
        }
        else if (counting && *str == '#')
            count++;
        else if (counting && (*str == '.' || *str == '\0'))
        {
            counting = 0;
            if (record[record_idx] != count)
                return 0;
            record_idx++;
            count = 0;
        }
        if (*str == '\0')
            return record_idx == record_len;
        else if (*str == '?')
            return 1;
        
        str++;
    }
}

int count_arrangements(char *str, int *record, int record_len, int idx)
{
      if (!valid(str, record, record_len))
          return 0;

    if (str[idx] == '\0')
    {
        int ret = valid(str, record, record_len) ? 1 : 0;
        //printf("%s: %d\n", str, ret);
        return ret;
    }
    else if (str[idx] == '?')
    {
        int total = 0;
        str[idx] = '.';
        total += count_arrangements(str, record, record_len, idx + 1);
        str[idx] = '#';
        total += count_arrangements(str, record, record_len, idx + 1);
        str[idx] = '?';
        return total;
    }
    else if (str[idx] == '.' || str[idx] == '#')
        return count_arrangements(str, record, record_len, idx + 1);
    else
    {
        printf("ERROR\n");
        exit(-1);
    }
}

int64_t main(int64_t argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    char buf[1024];

    int total = 0;
    while (!feof(f))
    {
        int record[256];
        int record_len = 0;

        fgets(buf, sizeof(buf), f);
        char *sep = strstr(buf, " ");
        char *tok = strtok(sep + 1, ",");
        while (tok)
        {
            record[record_len] = atoi(tok);
            record_len++;
            tok = strtok(NULL, ",");
        }

        *sep = '\0';
        //printf("%s\n", buf);

        int count = count_arrangements(buf, record, record_len, 0);
        //printf("%d\n", count);
        total += count;
    }
    printf("%d\n", total);
}

