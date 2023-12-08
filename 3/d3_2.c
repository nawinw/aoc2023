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

    int* nums = malloc(sizeof(int) * (size + 1));
    memset(nums, 0, size + 1);
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
                for (int dj = startj; dj < endj; dj++)
                {
                    nums[i * stride + dj] = n;
                }
            }
        }
    }

    fseek(f, 0, SEEK_SET);
    fread(buffer, 1, size, f);
    printf("%s\n", buffer);

    int sum = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (is_symbol(buffer[i * stride  + j]))
            {
                int n = 0;
                int ratio = 1;
                if (i > 0)
                {
                    for (int dj = j - 1; dj < j + 2; dj++)
                    {
                        if (dj >= 0 && dj < cols && isdigit(buffer[(i - 1) * stride + dj]))
                        {
                            if ((dj == j - 1) || (dj - 1 >= 0 && !isdigit(buffer[(i - 1) * stride + dj - 1])))
                            {
                                n++;
                                int v = nums[(i - 1) * stride + dj];
                                printf("a (%d, %d): %d\n", i - 1, dj, v);
                                ratio *= v;
                            }
                        }
                    }
                }
                if (i < rows)
                {
                    for (int dj = j - 1; dj < j + 2; dj++)
                    {
                        if (dj >= 0 && dj < cols && isdigit(buffer[(i + 1) * stride + dj]))
                        {
                            if ((dj == j - 1) || (dj - 1 >= 0 && !isdigit(buffer[(i + 1) * stride + dj - 1])))
                            {
                                int v = nums[(i + 1) * stride + dj];
                                printf("b (%d, %d): %d\n", i + 1, dj, v);
                                ratio *= v;
                                n++;
                            }
                        }
                    }
                }
                if (j > 1 && isdigit(buffer[i * stride + j - 1]))
                {
                    int v = nums[i * stride + j - 1];
                    printf("%d\n", v);
                    ratio *= v;
                    n++;
                }
                if (j < cols - 1 && isdigit(buffer[i * stride + j + 1]))
                {
                    int v = nums[i * stride + j + 1];
                    printf("%d\n", v);
                    ratio *= v;
                    n++;
                }

                if (n == 2)
                {
                    sum += ratio;
                }
            }
        }
    }
    printf("sum: %d\n", sum);
}