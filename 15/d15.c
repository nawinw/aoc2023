#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

int hash(char *str)
{
    int ret = 0;
    while(*str != '\0')
    {
        ret += *str;
        ret = (ret << 4) + ret;
        ret = ret & 0xFF;
        str++;
    }
    return ret;
}

int main(int argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    char *str = malloc(size + 1);
    fseek(f, 0, SEEK_SET);
    fread(str, 1, size, f);
    str[size] = '\0';

    int total = 0;
    char *tok = strtok(str, ",");
    while(tok)
    {
        total += hash(tok);
        tok = strtok(NULL, ",");
    }
    printf("%d\n", total);
}

