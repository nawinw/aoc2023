#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int64_t sort(int64_t* arr, int64_t len, int64_t *out)
{
    for (int64_t i = 0; i < len; i++)
        out[i] = i;

    for (int64_t i = 0; i < len; i++)
    {
        int64_t argmin = i;
        int64_t j = i + 1;
        for (; j < len; j++)
        {
            if (arr[out[j]] < arr[out[argmin]])
                argmin = j;
        }
        int64_t tmp = out[i];
        out[i] = out[argmin];
        out[argmin] = tmp;
    }
}

void grow(int64_t *arr, int64_t len, int64_t *idx)
{
    for (int64_t i = 1; i < len; i++)
    {
        int64_t delta = arr[idx[i]] - arr[idx[i - 1]] - 1;
        if (delta > 0)
        {
            for (int64_t j = i; j < len; j++)
                arr[idx[j]] += (delta * 999999);
        }
    }
}

int64_t main(int64_t argc, char* argv[])
{
    int64_t down[256];

    FILE *f = fopen(argv[1], "r");

    int64_t x[1024];
    int64_t y[1024];
    int64_t len = 0;

    int64_t cur_x = 0, cur_y = 0;
    while (!feof(f))
    {
        int64_t c = fgetc(f);

        if (c == '#')
        {
            x[len] = cur_x;
            y[len] = cur_y;
            len++;
        }

        cur_x++;
        if (c == '\n')
        {
            cur_y++;
            cur_x = 0;
        }
    }

    int64_t *idx = malloc(sizeof(int64_t) * len);
    sort(x, len, idx);
    grow(x, len, idx);

    sort(y, len, idx);
    grow(y, len, idx);

    int64_t total = 0;
    for (int64_t i = 0; i < len; i++)
    {
        for (int64_t j = 0; j < len; j++)
            total += abs(x[i] - x[j]) + abs(y[i] - y[j]);
    }
    printf("total: %ld\n", total / 2);
}