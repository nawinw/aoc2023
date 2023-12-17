#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3
int di[] = {-1, 0, 0, 1};
int dj[] = {0, -1, 1, 0};

#define MAX_MOVE 10
#define MAX_HEAT_LOSS (INT32_MAX / 2)

#define PRINT(prefix, x) printf(prefix "(i, j), (d, c, h): (%d, %d) (%d, %d, %d)\n", x.i, x.j, x.last_dir, x.dir_count, x.heat_loss)

typedef struct
{
    int heat_loss;
    int i, j;
    int last_dir;
    int dir_count;
} Pos;

#define QUEUE_SIZE 0x10000
Pos* queue;
int queue_size = 0;

void enqueue(Pos pos)
{
    int idx = queue_size++;
    queue[idx] = pos;

    while(idx > 0)
    {
        int parent_idx = (idx - 1) >> 1;
        if (queue[parent_idx].heat_loss > queue[idx].heat_loss)
        {
          Pos tmp = queue[parent_idx];
          queue[parent_idx] = queue[idx];
          queue[idx] = tmp;
        }
        else
            break;

        idx = parent_idx;
    };
}

Pos extract_min()
{
    Pos ret = {-1};
    if (queue_size > 0)
    {
        ret = queue[0];
        
        queue[0] = queue[--queue_size];
        queue[queue_size].heat_loss = MAX_HEAT_LOSS;

        int idx = 0;
        while(idx < queue_size)
        {
            int left_idx  = (idx << 1) + 1;
            int right_idx  = (idx << 1) + 2;
            int next_idx = queue[left_idx].heat_loss < queue[right_idx].heat_loss ? left_idx : right_idx;
            if (queue[next_idx].heat_loss >= queue[idx].heat_loss)
                break;

            Pos tmp = queue[idx];
            queue[idx] = queue[next_idx];
            queue[next_idx] = tmp;
            idx = next_idx;
        }
    }
    return ret;
}

#define DIM 160

int main(int argc, char* argv[])
{
    int32_t map[DIM][DIM];

    queue = malloc(sizeof(queue[0]) * QUEUE_SIZE);
    for (int i = 0; i < QUEUE_SIZE; i++)
        queue[i].heat_loss = MAX_HEAT_LOSS;

    FILE *f = fopen(argv[1], "r");
    int cols = 1;
    int rows = 1;
    int c;
    while((c = fgetc(f)) != EOF)
    {
        if (c != '\n')
            map[rows][cols++] = c - '0';
        else
        {
            cols = 1;
            rows++;
        }
    }
    cols++;
    rows += 2;

    for (int i = 0; i < rows; i++)
        map[i][0] = map[i][cols - 1] = MAX_HEAT_LOSS;
    for (int j = 0; j < cols; j++)
        map[0][j] = map[rows - 1][j] = MAX_HEAT_LOSS;

    // printf("%d %d\n", rows, cols);
    // for (int i = 0; i < rows; i++)
    // {
    //     for (int j = 0; j < cols; j++)
    //         printf("%c ", map[i][j] <= 9 ? map[i][j] + '0' : '.');

    //     printf("\n");
    // }

    char visited[4][MAX_MOVE + 1][DIM][DIM];
    memset(visited, 0, sizeof(visited));

    Pos start_pos;
    start_pos.i = 1;
    start_pos.j = 1;
    start_pos.dir_count = 4;
    start_pos.last_dir = -1;
    start_pos.heat_loss = 0;
    enqueue(start_pos);
    while(1)
    {
        Pos pos = extract_min();
        //PRINT("---", pos);

        if (pos.heat_loss == -1)
        {
            printf("ERROR\n");
            return -1;
        }

        if (pos.i == rows - 2 && pos.j == cols - 2 && pos.dir_count >= 4)
        {
            printf("%d\n", pos.heat_loss);
            return 0;
        }

        for (int i = 0; i < 4; i++)
        {
            if ((i ^ 0x3) != pos.last_dir && !(pos.last_dir == i && pos.dir_count == MAX_MOVE))
            {
                if (i != pos.last_dir && pos.dir_count < 4)
                    continue;
                    
                Pos new_pos;
                new_pos.i = pos.i + di[i];
                new_pos.j = pos.j + dj[i];
                new_pos.last_dir = i;
                new_pos.dir_count = i == pos.last_dir ? pos.dir_count + 1 : 1;
                if (!visited[new_pos.last_dir][new_pos.dir_count][new_pos.i][new_pos.j])
                {
                    new_pos.heat_loss = pos.heat_loss + map[new_pos.i][new_pos.j];
                    //PRINT("new", new_pos);
                    enqueue(new_pos);
                    visited[new_pos.last_dir][new_pos.dir_count][new_pos.i][new_pos.j] = 1;
                }
            }
        }
    }
}

