#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct
{
    int x, y;
} Vec;

Vec next(char tile, Vec pos, Vec v)
{
    Vec ret = {pos.x, pos.y};

    if (v.x == -1) // left
    {
        switch(tile)
        {
            case 'L':
                v.x = 0;
                v.y = -1;
                break;
            case 'F':
                v.x = 0;
                v.y = 1;
                break;
        }
    }
    else if (v.x == 1) // right
    {
        switch(tile)
        {
            case 'J':
                v.x = 0;
                v.y = -1;
                break;
            case '7':
                v.x = 0;
                v.y = 1;
                break;
        }
    }
    else if (v.y == -1) // up
    {
        switch(tile)
        {
            case '7':
                v.x = -1;
                v.y = 0;
                break;
            case 'F':
                v.x = 1;
                v.y = 0;
                break;
        }
    }
    else if (v.y == 1) // down
    {
        switch(tile)
        {
            case 'L':
                v.x = 1;
                v.y = 0;
                break;
            case 'J':
                v.x = -1;
                v.y = 0;
                break;
        }
    }
    else
    {
        printf("ERROR\n");
        exit(-1);
    }

    ret.x += v.x;
    ret.y += v.y;
    return ret;
}

int stride;

int pos2off(Vec pos)
{
    return pos.y * stride + pos.x;
}

Vec rot(Vec v)
{
    Vec ret;
    ret.x = -v.y;
    ret.y = v.x;
    return ret;
}

int main(int argc, char* argv[])
{
    int down[256];

    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *map = malloc(sz + 1);
    fread(map, 1, sz, f);
    map[sz] = '\0';

    stride = strstr(map, "\n") - map + 1;
    int cur = strstr(map, "S") - map;

    Vec pos[2], v[2];
    pos[0].x = pos[1].x = cur % stride;
    pos[0].y = pos[1].y = cur / stride;
    v[0].x = 0; v[0].y = 1;
    v[1].x = 0; v[1].y = -1;

    int steps = 0;
    while(1)
    {
        steps++;
        for (int i = 0; i < 2; i++)
        {
            Vec result = next(map[pos2off(pos[i])], pos[i], v[i]);
            v[i].x = result.x - pos[i].x;
            v[i].y = result.y - pos[i].y;
            pos[i].x = result.x;
            pos[i].y = result.y;

            if (pos[i].x == pos[1 - i].x && pos[i].y == pos[1 - i].y)
            {
                printf("%d\n", steps);
                exit(0);
            }
        }
    }

    printf("%s\n", map);
}