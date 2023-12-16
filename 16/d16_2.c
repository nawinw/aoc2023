#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8
#define ENERGIZED 16

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

int calc_energy(char *map, int rows, int cols, int stride, int init_state, int i0, int j0)
{
    int *state_map = (int*)malloc(sizeof(int) * rows * cols);
    memset(state_map, 0, sizeof(int) * rows * cols);

    while(1)
    {
        int changed = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int state;
                if (init_state)
                {
                    i = i0;
                    j = j0;
                    state = init_state;
                }
                else
                    state = state_map[i * cols + j];
                if (state & RIGHT && j < cols - 1)
                {
                    int *next = state_map + i * cols + j + 1;
                    int old = *next;
                    switch(map[i * stride + j + 1])
                    {
                        case '/':
                            *next |= UP;
                            break;
                        case '\\':
                            *next |= DOWN;
                            break;
                        case '|':
                            *next |= UP | DOWN;
                            break;
                        default:
                            *next |= RIGHT;
                    }
                    *next |= ENERGIZED;
                    changed |= *next != old;
                }
                if (state & LEFT && j > 0)
                {
                    int *next = state_map + i * cols + j - 1;
                    int old = *next;
                    switch(map[i * stride + j - 1])
                    {
                        case '/':
                            *next |= DOWN;
                            break;
                        case '\\':
                            *next |= UP;
                            break;
                        case '|':
                            *next |= UP | DOWN;
                            break;
                        default:
                            *next |= LEFT;
                    }
                    *next |= ENERGIZED;
                    changed |= *next != old;
                }
                if (state & UP && i > 0)
                {
                    int *next = state_map + (i - 1) * cols + j;
                    int old = *next;
                    switch(map[(i - 1) * stride + j])
                    {
                        case '/':
                            *next |= RIGHT;
                            break;
                        case '\\':
                            *next |= LEFT;
                            break;
                        case '-':
                            *next |= LEFT | RIGHT;
                            break;
                        default:
                            *next |= UP;
                    }
                    *next |= ENERGIZED;
                    changed |= *next != old;
                }
                if (state & DOWN && i < rows - 1)
                {
                    int *next = state_map + (i + 1) * cols + j;
                    int old = *next;
                    switch(map[(i + 1) * stride + j])
                    {
                        case '/':
                            *next |= LEFT;
                            break;
                        case '\\':
                            *next |= RIGHT;
                            break;
                        case '-':
                            *next |= LEFT | RIGHT;
                            break;
                        default:
                            *next |= DOWN;
                    }
                    *next |= ENERGIZED;
                    changed |= *next != old;
                }
                if (init_state)
                {
                    i = 0;
                    j = 0;
                    init_state = 0;
                }
            }
        }
        if (!changed)
            break;
    }

    int total = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (state_map[i * cols + j] & ENERGIZED)
                total++;
        }
    }

    free(state_map);
    return total;
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

    printf("%d %d\n", rows, cols);
    printf("%s\n", map);


    int max = 0;
    for (int j = 0; j < cols; j++)
    {
        max = MAX(max, calc_energy(map, rows, cols, stride, DOWN, -1, j));
        max = MAX(max, calc_energy(map, rows, cols, stride, UP, rows, j));
    }
    for (int i = 0; i < rows; i++)
    {
        max = MAX(max, calc_energy(map, rows, cols, stride, RIGHT, i, -1));
        max = MAX(max, calc_energy(map, rows, cols, stride, LEFT, i, cols));
    }
    printf("%d\n", max);
}

