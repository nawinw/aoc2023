#include <stdio.h>
#include <stdlib.h>

#define RED 12
#define GREEN 13
#define BLUE 14

int parseNum(char** buffer)
{
    int n = 0;
    while(**buffer >= '0' && **buffer <= '9')
    {
        n *= 10;
        n += **buffer - '0';
        (*buffer)++;
    }
    return n;
}

int check(int r, int g, int b)
{
    if (r < RED && g < GREEN && b < BLUE)
        return 1;

    return 0;
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

    int sum = 0;
    int id = -1;
    int valid = 1;
    int r = 0, g = 0, b = 0;

    while(*buffer != '\0')
    {
        int check = 0;
        switch(*buffer)
        {
            case 'G':
                buffer += 5;
                if (id != -1 && valid)
                    sum += id;
                id = parseNum(&buffer);
                valid = 1;
                buffer += 2;
                break;

            case ',':
                buffer += 2;
                break;

            case ';':
                check = 1;
                buffer += 2;
                break;

            case '\n':
                check = 1;
                buffer++;
                break;

            case '\0':
                check = 1;
                break;

            default:
            {
                int n = parseNum(&buffer);
                buffer++;
                switch (*buffer)
                {
                    case 'r':
                        r = n;
                        buffer += 3;
                        break;
                    case 'g':
                        g = n;
                        buffer += 5;
                        break;
                    case 'b':
                        b = n;
                        buffer += 4;
                        break;
                }
            }
        }
        if (check)
        {
            if (r > RED || g > GREEN || b > BLUE)
                valid = 0;
            r = 0; g = 0; b = 0;
        }
    }
    if (valid)
        sum += id;
    printf("Sum: %d\n", sum);
}