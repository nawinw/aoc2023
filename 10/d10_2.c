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

int stride, height;

int pos2off(Vec pos)
{
    return pos.y * stride + pos.x;
}

Vec rot(Vec v)
{
    Vec ret;
#if 0
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
        map[offset] = 'I';
        if (pos.x > 0)
            fill(map, make_vec(pos.x - 1, pos.y));
        if (pos.y > 0)
            fill(map, make_vec(pos.x, pos.y - 1));
        if (pos.x < stride - 1)
            fill(map, make_vec(pos.x + 1, pos.y));
        if (pos.y < height)
            fill(map, make_vec(pos.x, pos.y + 1));
    }
}

int main(int argc, char* argv[])
{
    int down[256];

    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *map = malloc(sz + 1);
    char *is_route = malloc(sz + 1);
    memset(is_route, 0, sz + 1);
    fread(map, 1, sz, f);
    map[sz] = '\0';

    stride = strstr(map, "\n") - map + 1;
    height = (sz + 1) / stride;
    printf("%d\n", height);
    int cur = strstr(map, "S") - map;

    Vec pos[2], v[2];
    pos[0].x = pos[1].x = cur % stride;
    pos[0].y = pos[1].y = cur / stride;
    v[0].x = 0; v[0].y = -1;
    v[1].x = 0; v[1].y = 1;

    while(1)
    {
        int i = 0;

        is_route[pos2off(pos[i])] = 1;

        Vec result = next(map[pos2off(pos[i])], pos[i], v[i]);
        v[i].x = result.x - pos[i].x;
        v[i].y = result.y - pos[i].y;

        pos[i].x = result.x;
        pos[i].y = result.y;

        is_route[pos2off(pos[i])] = 1;

        if (pos[i].x == pos[1 - i].x && pos[i].y == pos[1 - i].y)
            break;
    }

    for (int i = 0; i < sz; i++)
    {
        if (!is_route[i] && map[i] != '\n')
            map[i] = '.';
    }

    pos[0].x = pos[1].x = cur % stride;
    pos[0].y = pos[1].y = cur / stride;
    v[0].x = 0; v[0].y = -1;
    v[1].x = 0; v[1].y = 1;
    while(1)
    {
        int i = 0;
        
        Vec delta = rot(v[i]);
        fill(map, make_vec(pos[i].x + delta.x, pos[i].y + delta.y));

        Vec result = next(map[pos2off(pos[i])], pos[i], v[i]);
        v[i].x = result.x - pos[i].x;
        v[i].y = result.y - pos[i].y;

        delta = rot(v[i]);
        fill(map, make_vec(pos[i].x + delta.x, pos[i].y + delta.y));

        pos[i].x = result.x;
        pos[i].y = result.y;

        if (pos[i].x == pos[1 - i].x && pos[i].y == pos[1 - i].y)
            break;
    }
    int count = 0;
    for (int i = 0; i < sz; i++)
    {
        if (map[i] == 'I')
            count++;
    }
    printf("%s\n", map);
    printf("%d\n", count);
}