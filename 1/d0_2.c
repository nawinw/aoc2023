#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(size);
    fread(buffer, 1, size, f);
    char *digits[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int lengths[] = {3,3,5,4,4,3,5,5,4};
    for(long i = 0; i < size; i++)
    {
        long remaining = size - i;
        if (buffer[i] == '\n')
            continue;
        for (int j = 0; j < 9; j++)
        {
            if (remaining >= lengths[j] && memcmp(buffer + i, digits[j], lengths[j]) == 0)
            {
                buffer[i] = j + '1';
            }
        }
    }

    int total = 0;
    char value;
    int state = 1;
    for (long i = 0; i < size; i++)
    {
        char c = buffer[i];
        char accept = (0xf0 & c) == 0x30;
        if (accept)
        {
            value = c & 0x0f;
            if (state)
            {
                total += value * 10;
                state = 0;
            }
        }
        else if (c == '\n' && !state)
        {
            total += value;
            state = 1;
        }
    }
    total += value;
    printf("%d\n", total);
}