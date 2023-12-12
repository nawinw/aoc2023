#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#define NUM_TYPES 7
#define NUM_VALUES 13
#define MAX_HANDS (NUM_VALUES*NUM_VALUES*NUM_VALUES*NUM_VALUES*NUM_VALUES)
#define MAX_SLOTS (MAX_HANDS * NUM_TYPES)


int char2val['Z'];

int hand2slot(char *hand)
{
    int matches[NUM_VALUES];
    memset(matches, 0, sizeof(matches));

    int total = 0;
    while(*hand)
    {
        int val = char2val[*hand];
        matches[val]++;
        total = (total * NUM_VALUES) + val;
        hand++;
    }

    int max_matches[2];
    for (int i = 0; i < 2; i++)
    {
        int argmax;
        max_matches[i] = -1;
        for (int j = 1; j < NUM_VALUES; j++)
        {
            if (matches[j] > max_matches[i])
            {
                argmax = j;
                max_matches[i] = matches[j];
            }
        }
        max_matches[i] += matches[0];
        matches[0] = 0;
        matches[argmax] = 0;
    }

    int type = 0;
    if (max_matches[0] == 5)
        type = 6;
    else if (max_matches[0] == 4)
        type = 5;
    else if (max_matches[0] == 3)
        type = max_matches[1] == 2 ? 4 : 3;
    else if (max_matches[0] == 2)
        type = max_matches[1] == 2 ? 2 : 1;
    
    return type * MAX_HANDS + total;
}

int main(int argc, char *argv[])
{
    char2val['J'] = 0;
    char2val['2'] = 1;
    char2val['3'] = 2;
    char2val['4'] = 3;
    char2val['5'] = 4;
    char2val['6'] = 5;
    char2val['7'] = 6;
    char2val['8'] = 7;
    char2val['9'] = 8;
    char2val['T'] = 9;
    char2val['Q'] = 10;
    char2val['K'] = 11;
    char2val['A'] = 12;

    size_t sz = sizeof(int) * MAX_SLOTS;
    int* slots = malloc(sz);
    memset(slots, 0, sz);

    FILE *f = fopen(argv[1], "r");
    while (!feof(f))
    {
        char hand[8];
        int bid;
        fscanf(f, "%s %d\n", hand, &bid);
        printf("%s %d %d\n", hand, bid, hand2slot(hand));
        slots[hand2slot(hand)] = bid;
    }

    int rank = 1;
    uint64_t total = 0;
    for (int i = 0; i < MAX_SLOTS; i++)
    {
        if (slots[i] != 0)
        {
            total += rank * slots[i];
            rank++;
        }
    }
    printf("%lu\n", total);
}