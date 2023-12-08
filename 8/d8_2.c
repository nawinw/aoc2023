#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct
{
    char label[4];
    int left;
    int right;
} Node ;

int label2int(char *label)
{
    return (int)(label[0]-'A') + (int)(label[1]-'A') * 26 + (int)(label[2]-'A') * 26 * 26;
}

int main(int argc, char* argv[])
{
    Node *nodes = malloc(sizeof(Node) * label2int("ZZZ"));

    FILE *f = fopen(argv[1], "r");
    char path[1024];
    fscanf(f, "%s\n\n", path);

    printf("%s\n", path);

    char buf[256];
    int numA = 0;
    int cur[256];
    uint64_t last[256];
    memset(last, 0, sizeof(last));
    while (!feof(f))
    {
        fgets(buf, sizeof(buf), f);
        printf("%s\n", buf);
        int idx = label2int(buf);
        int left = label2int(buf + 7);
        int right = label2int(buf + 12);
        memcpy(nodes[idx].label, buf, 3);
        nodes[idx].label[3] = '\0';
        nodes[idx].left = left;
        nodes[idx].right = right;

        if (buf[2] == 'A')
        {
            cur[numA] = idx;
            last[numA] = -1;
            numA++;
        }
    }

    uint64_t steps = 0;
    uint64_t local_steps = 0;
    uint64_t pathlen = strlen(path);
    while(1)
    {
        int goal = 1;
        for (int i = 0; i < numA; i++)
        {
            if (nodes[cur[i]].label[2] == 'Z')
            {
                if (last[i] == -1)
                {
                    last[i] = steps;
                }
                else
                {
                    uint64_t delta = steps - last[i];
                    uint64_t c = steps % delta;
                    printf("[%d] %llu + %llu x\n", i, c, delta);
                    last[i] = steps;
                }
                goal = 0;
            }
        }

        for (int i = 0; i < numA; i++)
        {
            //printf("%s = (%s, %s)\n", nodes[cur[i]].label, nodes[nodes[cur[i]].left].label, nodes[nodes[cur[i]].right].label);
            cur[i] = path[local_steps] == 'L' ? nodes[cur[i]].left : nodes[cur[i]].right;
        }
        steps++;
        local_steps++;
        if (local_steps >= pathlen)
            local_steps = 0;
    }
}