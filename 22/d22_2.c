#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct
{
    int x0, y0, z0;
    int x1, y1, z1;
} Points;

typedef struct
{
    int x, y;
} Vec;


typedef union
{
    int n[6];
    Points p;
} Brick;

void sort(Brick *brick, int num_bricks)
{
    for (int i = 0; i < num_bricks; i++)
    {
        for (int j = i + 1; j < num_bricks; j++)
        {
            if (MIN(brick[j].p.z0, brick[j].p.z1) < MIN(brick[i].p.z0, brick[i].p.z1))
            {
                Brick tmp = brick[i];
                brick[i] = brick[j];
                brick[j] = tmp;
            }
        }
    }
}



int intersect(Brick brick0, Brick brick1)
{
    // Ugly brute force line intersection
    int dx0 = brick0.p.x1 - brick0.p.x0;
    int dy0 = brick0.p.y1 - brick0.p.y0;
    int count0 = MAX(dx0, dy0);
    if (count0)
    {
        dx0 /= count0;
        dy0 /= count0;
    }
    int dx1 = brick1.p.x1 - brick1.p.x0;
    int dy1 = brick1.p.y1 - brick1.p.y0;
    int count1 = MAX(dx1, dy1);
    if (count1)
    {
        dx1 /= count1;
        dy1 /= count1;
    }
    for (int i = 0; i <= count0; i++)
    {
        for (int j = 0; j <= count1; j++)
            if (brick0.p.x0 + dx0 * i == brick1.p.x0 + dx1 * j && brick0.p.y0 + dy0 * i == brick1.p.y0 + dy1 * j)
                return 1;
    }
    return 0;
}

void fall(Brick *bricks, int num_bricks)
{
    for (int i = 0; i < num_bricks; i++)
    {
        int ground = 0;
        int zi = MIN(bricks[i].p.z0, bricks[i].p.z1);
        for (int j = 0; j < num_bricks; j++)
        {
            int zj = MAX(bricks[j].p.z0, bricks[j].p.z1);
            if (i != j && zi > zj && intersect(bricks[i], bricks[j]))
                ground = MAX(ground, zj);
        }
        int delta = zi - ground - 1;
        bricks[i].p.z0 -= delta;
        bricks[i].p.z1 -= delta;
    }
}

void remove_brick_(int *support, int num_bricks, int idx)
{
    for(int i = 0; i < num_bricks; i++)
    {
        if (idx != i && support[idx * num_bricks + i])
        {
            support[idx * num_bricks + i] = 0;

            int num_supports = 0;
            for (int j = 0; j < num_bricks; j++)
            {
                if (j != i && support[j * num_bricks + i])
                {
                    num_supports++;
                    break;
                }
            }
            if (!num_supports)
                remove_brick_(support, num_bricks, i);
        }
    }
}

int remove_brick(Brick *bricks, int *support, int num_bricks, int idx)
{
    int *new_support = malloc(sizeof(int) * num_bricks * num_bricks);
    memcpy(new_support, support, sizeof(int) * num_bricks * num_bricks);

    remove_brick_(new_support, num_bricks, idx);

    int removed = 0;
    for (int i = 0; i < num_bricks; i++)
    {
        if (i != idx && MIN(bricks[i].p.z0, bricks[i].p.z1) > 1)
        {
            int num_supports = 0;
            for (int j = 0; j < num_bricks; j++)
            {
                if (new_support[j * num_bricks + i])
                {
                    num_supports++;
                    break;
                }
            }
            if (!num_supports)
                removed++;
        }
    }

    return removed;
}

int main(int argc, char* argv[])
{
    Brick *bricks = malloc(sizeof(Brick) * 2048);
    int num_bricks = 0;

    FILE *f = fopen(argv[1], "r");
    while(!feof(f))
    {
        char buf[64];
        fgets(buf, sizeof(buf), f);
        char *tok = strtok(buf, ",~\n");
        for (int i = 0; i < 6; i++)
        {
            bricks[num_bricks].n[i] = atoi(tok);
            tok = strtok(NULL, ",~\n");
        }
        num_bricks++;
    }

    int *support = malloc(sizeof(int) * num_bricks * num_bricks);
    memset(support, 0, sizeof(int) * num_bricks * num_bricks);

    for (int i = 0; i < num_bricks; i++)
    {
        Brick *brick = bricks + i;
        printf("%c :: %d,%d,%d~%d,%d,%d\n", i + 'A', brick->p.x0, brick->p.y0, brick->p.z0, brick->p.x1, brick->p.y1, brick->p.z1);
    }

    sort(bricks, num_bricks);
    fall(bricks, num_bricks);

    //i,j = 1 means brick i supports brick j
    for (int i = 0; i < num_bricks; i++)
    {
        for (int j = 0; j < num_bricks; j++)
        {
            if (i != j && intersect(bricks[i], bricks[j]) && MIN(bricks[i].p.z0, bricks[i].p.z1) == MAX(bricks[j].p.z0, bricks[j].p.z1) + 1)
            {
                // printf("%c supports %c\n", j + 'A', i + 'A');
                support[j * num_bricks + i] = 1;
            }
        }
    }

    int total = 0;
    for (int i = 0; i < num_bricks; i++)
    {
        int subtotal = remove_brick(bricks, support, num_bricks, i);
        //printf("%d\n", subtotal);
        total += subtotal;
    }
    printf("%d\n", total);

    // for (int i = 0; i < num_bricks; i++)
    // {
    //     Brick *brick = bricks + i;
    //     printf("%c :: %d,%d,%d~%d,%d,%d :: %d\n", i + 'A', brick->p.x0, brick->p.y0, brick->p.z0, brick->p.x1, brick->p.y1, brick->p.z1, num_supports[i]);
    // }
}

