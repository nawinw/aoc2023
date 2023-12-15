#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

struct Lens
{
    char label[16];
    int focal_length;
    struct Lens *next;
};

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

int score(int box, struct Lens* lens, int idx, int *slots)
{
    if (!lens)
        return 0;

    (*slots)++;
    int ret = score(box, lens->next, idx + 1, slots);
    int slot = *slots - idx;
    ret += (box + 1) * slot * lens->focal_length;
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

    struct Lens boxes[256];
    memset(&boxes, 0, sizeof(boxes));

    char *label = strtok(str, ",");
    char buf[256];
    while(label)
    {
        char *sep;
        if (sep = strstr(label, "="))
        {
            *sep = '\0';
            int box = hash(label);
            int focal_length = *(sep + 1) - '0';
            struct Lens *head = boxes[box].next;
            while (head)
            {
                if (strcmp(head->label, label) == 0)
                {
                    head->focal_length = focal_length;
                    break;
                }
                head = head->next;
            }
            if (!head)
            {
                struct Lens* new_lens = (struct Lens*)malloc(sizeof(struct Lens));
                strcpy(new_lens->label, label);
                new_lens->focal_length = focal_length;
                new_lens->next = boxes[box].next;
                boxes[box].next = new_lens;
            }
        }
        else if (sep = strstr(label , "-"))
        {
            *sep = '\0';
            int box = hash(label);
            struct Lens *head = boxes + box;
            while (head)
            {
                if (head->next && strcmp(head->next->label, label) == 0)
                {
                    head->next = head->next->next;
                    break;
                }
                head = head->next;
            }
        }
        else
        {
            printf("ERROR\n");
            return -1;
        }
        label = strtok(NULL, ",");
    }

    int total = 0;
    for (int i = 0; i < 256; i++)
    {
        int slots = 0;
        total += score(i, boxes[i].next, 0, &slots);
    }
    printf("%d\n", total);
}

