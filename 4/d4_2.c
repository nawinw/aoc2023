
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    int counts[256];
    memset(counts, 0, sizeof(counts));
    int card = 0;
    while (!feof(f))
    {
        if (counts[card] == 0)
            counts[card] = 1;

        int winning[256];
        int wcount = 0;
        int numbers[256];
        int ncount = 0;

        char buf[1024];
        fgets(buf, sizeof(buf), f);
        char *tok = strtok(buf + 8, " ");
        int state = 0;
        while (tok)
        {
            if (tok[0] == '|')
                state = 1;
            else if (state == 0)
                winning[wcount++] = atoi(tok);
            else if (state == 1)
                numbers[ncount++] = atoi(tok);
            tok = strtok(NULL, " ");
        }

        int score = 0;
        for (int i = 0; i < wcount; i++)
        {
            for (int j = 0; j < ncount; j++)
            {
                if (winning[i] == numbers[j])
                    score++;  
            }
        }
        for (int i = 0; i < score; i++)
        {
            if (counts[card + i + 1] == 0)
                counts[card + i + 1] = 1;
            counts[card + i + 1] += counts[card];
        }
        card++;
    }
    int total = 0;
    for (int i = 0; i < 256; i++)
    {
        printf("%d\n", counts[i]);
        total += counts[i];
    }
    printf("%d\n", total);
}