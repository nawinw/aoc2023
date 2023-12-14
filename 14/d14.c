#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

void tilt(char *map, int rows, int cols, int elem_size, int stride, int offset)
{
    for (int j = 0; j < cols; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            if (map[i * stride + j * elem_size + offset] == '.')
            {
                for (int k = i + 1; k < rows; k++)
                {
                    if (map[k * stride + j * elem_size + offset] == 'O')
                    {
                        map[i * stride + j * elem_size + offset] = 'O';
                        map[k * stride + j * elem_size + offset] = '.';
                        break;
                    }
                    else if (map[k * stride + j * elem_size + offset] == '#')
                    {
                        i = k;
                        break;
                    }
                }
            }
        }
    }
}

int calc(char *map, int rows, int cols, int elem_size, int stride)
{
    int load = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            load += map[i * stride + j] == 'O' ? rows - i : 0;
    }
    return load;
}

int main(int argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    char *map = malloc(size + 1);
    fseek(f, 0, SEEK_SET);
    fread(map, 1, size, f);
    map[size] = '\0';

    int cols = strstr(map, "\n") - map;
    int stride = cols + 1;
    int rows = (size + 1) / stride;

    for (int i = 0; i < 1000000000; i++)
    {
        tilt(map, rows, cols, 1, stride, 0); // tilt north
        tilt(map, cols, rows, stride, 1, 0); // tilt west
        tilt(map, rows, cols, 1, -stride, (rows - 1) * stride); // tilt south
        tilt(map, cols, rows, stride, -1, cols - 1); // tilt east
        int load = calc(map, rows, cols, 1, stride);
        printf("%d %d\n", i % 7, load);
    }

    printf("%s\n", map);
}

