#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

typedef struct 
{
    char dir;
    int64_t dist;
} Data;

typedef struct 
{
    int64_t i0, j0, i1, j1;
} Line;

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

        data[num_data].dir = color[7] - '0';
        color[7] = '\0';
        data[num_data].dist = strtol(color + 2, NULL, 16);
        num_data++;
    }

    Line *lines = malloc(sizeof(Line) * 2048);
    int num_lines = 0;

    int64_t cur_i = 0, cur_j = 0;
    int min_i = cur_i, max_i = cur_i;
    for (int idx = 0; idx < num_data; idx++)
    {
        lines[num_lines] = (Line){cur_i, cur_j, cur_i, cur_j};
        switch(data[idx].dir)
        {
            case UP:
                cur_i -= data[idx].dist;
                lines[num_lines++].i0  = cur_i;
                break;

            case DOWN:
                cur_i += data[idx].dist;
                lines[num_lines++].i1 = cur_i;
                break;

            case RIGHT:
                cur_j += data[idx].dist;
                break;

            case LEFT:
                cur_j -= data[idx].dist;
                break;

            default:
                printf("ERROR\n");
                return -1;
        }
        min_i = MIN(min_i, cur_i);
        max_i = MAX(max_i, cur_i);
    }

    // ##
    // ##
    // num_lines = 4;
    // lines[0].i0 = 0;
    // lines[0].j0 = 0;
    // lines[0].i1 = 1;
    // lines[0].j1 = 0;

    // lines[1].i0 = 0;
    // lines[1].j0 = 1;
    // lines[1].i1 = 1;
    // lines[1].j1 = 1;
    // min_i = 0;
    // max_i = 1;


    // ##
    // ###
    // ###
    // num_lines = 3;
    // lines[0].i0 = 0;
    // lines[0].j0 = 0;
    // lines[0].i1 = 2;
    // lines[0].j1 = 0;

    // lines[1].i0 = 0;
    // lines[1].j0 = 1;
    // lines[1].i1 = 1;
    // lines[1].j1 = 1;

    // lines[2].i0 = 1;
    // lines[2].j0 = 2;
    // lines[2].i1 = 2;
    // lines[2].j1 = 2;
    // min_i = 0;
    // max_i = 2;

    for (int i = 0; i < num_lines; i++)
    {
        for (int j = i + 1; j < num_lines; j++)
        {
            if (lines[j].j0 < lines[i].j0)
            {
                Line tmp = lines[i];
                lines[i] = lines[j];
                lines[j] = tmp;
            }
        }
    }

    int64_t total = 0;
    for (int64_t i = min_i; i <= max_i; i++)
    {
        int inside = 0;
        int prev_inside = 0;
        int64_t prev_j;
        int64_t prev_i;
        int vertex_hits = 0;
        for (int64_t idx = 0; idx < num_lines; idx++)
        {
            if (i >= lines[idx].i0 && i <= lines[idx].i1)
            {
                int new_i = i;
                if (i == lines[idx].i0)
                {
                    if (vertex_hits == 0)
                        prev_inside = inside;
                    vertex_hits++;
                    new_i = lines[idx].i1;                    
                }
                else if (i == lines[idx].i1)
                {
                    if (vertex_hits == 0)
                        prev_inside = inside;
                    vertex_hits++;
                    new_i = lines[idx].i0;                    
                }
                else
                    vertex_hits = 0;

                int old_inside = inside;
                if (vertex_hits == 2)
                {
                    inside = (prev_i - i) * (new_i - i) >= 0 ? prev_inside : !prev_inside;
                    vertex_hits = 0;
                }
                else if (vertex_hits == 1)
                    inside = 1;
                else
                    inside = !inside;

                if (!inside && old_inside)
                    total += lines[idx].j0 - prev_j + 1;
                else if (inside && !old_inside)
                    prev_j = lines[idx].j1;

                prev_i = new_i;
            }
        }
    }

    printf("%ld\n", total);
}