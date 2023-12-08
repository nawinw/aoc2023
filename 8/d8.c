#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int left;
    int right;
} Node ;

int label2int(char *label)
{
    return (int)label[0] + label[1] * 26 + label[2] * 26 * 26;
}

int main(int argc, char* argv[])
{
    int ZZZ = label2int("ZZZ");
    Node *nodes = malloc(sizeof(Node) * ZZZ);

    FILE *f = fopen(argv[1], "r");
    char path[1024];
    fscanf(f, "%s\n\n", path);

    printf("%s\n", path);

    char buf[256];
    while (!feof(f))
    {
        fgets(buf, sizeof(buf), f);
        int idx = label2int(buf);
        int left = label2int(buf + 7);
        int right = label2int(buf + 12);
        nodes[idx].left = left;
        nodes[idx].right = right;
    }

    int steps = 0;
    int pathlen = strlen(path);
    int cur = label2int("AAA");
    while(cur != ZZZ)
    {
        cur = path[steps % pathlen] == 'L' ? nodes[cur].left : nodes[cur].right;
        steps++;
    }
    printf("%d\n", steps);
}