#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct
{
    int x, y;
} Vec;

int cols = 512;
int rows = 512;

int pos2off(Vec pos)
{
    return pos.y * cols + pos.x;
}

Vec rot(Vec v)
{
    Vec ret;
#if 1
    ret.x = -v.y;
    ret.y = v.x;
#else
    ret.x = v.y;
    ret.y = -v.x;
#endif
    return ret;
}

Vec make_vec(int x, int y)
{
    Vec ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

void fill(char *map, Vec pos)
{
    int offset = pos2off(pos);
    if (map[offset] == '.')
    {
        map[offset] = '#';
        if (pos.x > 0)
            fill(map, make_vec(pos.x - 1, pos.y));
        if (pos.y > 0)
            fill(map, make_vec(pos.x, pos.y - 1));
        if (pos.x < cols - 1)
            fill(map, make_vec(pos.x + 1, pos.y));
        if (pos.y < rows - 1)
            fill(map, make_vec(pos.x, pos.y + 1));
    }
}

typedef struct 
{
    char dir;
    int dist;
} Data;

int main(int argc, char* argv[])
{
    Data *data = malloc(sizeof(Data) * 1024);
    int num_data = 0;

    FILE *f = fopen(argv[1], "r");
    while (!feof(f))
    {
        char dir;
        int dist;
        char color[16];

        fscanf(f, "%c %d %s\n", &dir, &dist, color);
        data[num_data++] = (Data){dir, dist};
    }

    char *map = malloc(rows * cols + 1);
    memset(map, '.', rows * cols);
    int i = rows / 2;
    int j = cols / 2;
    int min_i = i, min_j = j, max_i = i, max_j = j;
    for (int state = 0; state < 2; state++)
    {
        for (int idx = 0; idx < num_data; idx++)
        {
            for (int d = 0; d < data[idx].dist; d++)
            {
                int old_i = i;
                int old_j = j;
                map[i * cols + j] = '#';
                switch(data[idx].dir)
                {
                    case 'U':
                        i--;
                        break;
                    case 'D':
                        i++;
                        break;
                    case 'L':
                        j--;
                        break;
                    case 'R':
                        j++;
                        break;
                    default:
                        printf("ERROR\n");
                        return -1;
                }
                if (state)
                {
                    Vec v = rot((Vec){j - old_j, i - old_i});
                    fill(map, (Vec){j + v.x, i + v.y});
                }
            }
            min_i = MIN(min_i, i);
            min_j = MIN(min_j, j);
            max_i = MAX(max_i, i);
            max_j = MAX(max_j, j);
        }
    }

    // for (int i = min_i; i <= max_i; i++)
    // {
    //     for (int j = min_j; j <= max_j; j++)
    //         printf("%c ", map[i * cols + j]);
    //     printf("\n");
    // }

    int count = 0;
    for (int i = 0; i < rows * cols; i++)
    {
        if (map[i] == '#')
            count++;
    }
    printf("%d\n", count);
}