#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

int count_reflect(char *map, int rows, int cols, int elem_size, int stride)
{
    for (int i = 1; i < rows; i++)
    {
        int diff = 0;
        for (int delta = 1; delta < rows; delta++)
        {
            int i0 = i - delta;
            int i1 = i + delta - 1;
            if (i0 < 0 || i1 >= rows)
                continue;

            for (int j = 0; j < cols; j++)
            {
                if (map[i0 * stride + j * elem_size] != map[i1 * stride + j * elem_size])
                    diff++;
            }
        }
        if (diff == 0)
            return i;
    }

    return 0;
}

int64_t main(int64_t argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    char *buf = malloc(size + 1);
    fseek(f, 0, SEEK_SET);
    fread(buf, 1, size, f);
    buf[size] = '\0';

    char *map = buf;
    int total = 0;
    while(1)
    {
        int cols = strstr(map, "\n") - map;
        char* end = strstr(map, "\n\n");
        int last = 0;
        if (end == NULL)
        {
            end = buf + size;
            last = 1;
        }
        int rows = (end - map + 1) / (cols + 1);
        
        int count = count_reflect(map, rows, cols, 1, cols + 1) * 100 + count_reflect(map, cols, rows, cols + 1, 1);
        total += count;
        map += rows * (cols + 1) + 1;
        if (last)
            break;
    }
    printf("%d\n", total);
}

