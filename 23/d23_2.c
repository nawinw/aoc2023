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
#define NUM_COMBINATIONS 16

#define STACK_SIZE 65535
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

char di[] = {-1, 1, 0, 0};
char dj[] = {0, 0, -1, 1};

typedef struct
{
    int i, j;
    int len;
} Pos;

typedef struct
{
    Pos *data;
    int tail;
    int head;
} Stack;


Pos *stack, *path;
int stack_tail = 0;
int stack_head = 0;

int empty(Stack stack)
{
    return stack.tail == stack.head;
}

void push(Pos pos)
{
    stack[stack_head] = pos;
    stack_head++;
}

Pos peek()
{
    return stack[stack_head - 1];
}

Pos pop()
{
    Pos ret = peek();
    stack_head--;
    return ret;
}

int find_longest(char *map, int rows, int cols, int stride, char *visited, int *max, int i, int j, int len)
{
    if (i == rows - 1 && j == cols - 2)
    {
        // if (len == 4618)
        // {
        //     for (int i = 0; i < rows; i++)
        //     {
        //         for (int j = 0; j < cols; j++)
        //             printf("%c", visited[i * cols + j] ? 'O' : map[i * stride + j]);
        //         printf("\n");
        //     }
        //     printf("\n");
        // }
        return len;
    }

    int longest = 0;
    visited[i * cols + j] = 1;

    int di[4];
    int dj[4];
    int num_dir = 1;
    di[0] = 1;
    dj[0] = 0;
    di[1] = -1;
    dj[1] = 0;
    di[2] = 0;
    dj[2] = 1;
    di[3] = 0;
    dj[3] = -1;
    num_dir = 4;

    for (int n = 0; n < num_dir; n++)
    {
        int new_i = i + di[n];
        int new_j = j + dj[n];
        if (new_i >= 0 && new_i < rows && new_j >= 0 && new_j < cols && !visited[new_i * cols + new_j] && map[new_i * stride + new_j] != '#')
        {
            // int max_idx = 0;
            // if (new_i == 0 || visited[(new_i - 1) * cols + new_j])
            //     max_idx |= UP;
            // if (new_i == rows - 1 || visited[(new_i + 1) * cols + new_j])
            //     max_idx |= DOWN;
            // if (new_j == 0 || visited[new_i * cols + new_j - 1])
            //     max_idx |= LEFT;
            // if (new_j == cols - 1 || visited[new_i * cols + new_j + 1])
            //     max_idx |= RIGHT;

            int new_len = len + 1;
            //if (max[max_idx * (rows * cols) + new_i * cols + new_j] < new_len)
            {
                // max[max_idx * (rows * cols) + new_i * cols + new_j] = new_len;
                int new_longest = find_longest(map, rows, cols, stride, visited, max, new_i, new_j, new_len);
                longest = MAX(longest, new_longest);
            }
        }
    }
    visited[i * cols + j] = 0;

    return longest;
}

//5034 too low

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

    int *max = malloc(NUM_COMBINATIONS * sizeof(int) * rows * cols);
    memset(max, 0, NUM_COMBINATIONS * sizeof(int) * rows * cols);

    char *visited = malloc(rows * cols);
    memset(visited, 0, rows * cols);

    int longest = find_longest(map, rows, cols, stride, visited, max, 0, 1, 0);
    printf("%d\n", longest);
}

