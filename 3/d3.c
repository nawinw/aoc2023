#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int is_symbol(char c)
{
    if (c == '.' || isdigit(c))
        return 0;

    return 1;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';

    int cols = (int)(strstr(buffer, "\n") - buffer);
    int stride = cols + 1;
    int rows = (size + 1) / stride;
    int sum = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char c = buffer[i * stride  + j];
            int n = 0;
            if (isdigit(c))
            {
                int startj = j;
                int endj;
                for (endj = startj; endj < cols; endj++)
                {
                    c = buffer[i * stride  + endj];
                    if (isdigit(c))
                    {
                        buffer[i * stride  + endj] = '.';
                        n *= 10;
                        n += c - '0';
                    }
                    else
                        break;
                }
                int exit = 0;
                for (int ci = i - 1; ci < rows && ci < i + 2; ci++)
                {
                    for (int cj = startj - 1; cj < cols && cj < endj + 1; cj++)
                    {
                        if (ci >= 0 && cj >= 0 && ci < rows && cj < cols && is_symbol(buffer[ci * stride  + cj]))
                        {
                            printf("%d\n", n);
                            sum += n;
                            exit = 1;
                            break;
                        }
                    }
                    if (exit)
                        break;
                }
            }
        }
    }
    printf("sum: %d\n", sum);
}