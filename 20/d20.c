#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include <regex.h>

#define LOW 0
#define HIGH 1
#define OFF 0
#define ON 1

#define QUEUE_SIZE 1024


typedef struct
{
    int src;
    int dest;
    int pulse;
} Msg;

typedef struct Module
{
    char name[16];
    char prefix;
    int state;
} Module;

Module modules[64];
int mat[64][64];
int sent[64][64];
uint64_t count[] = {0, 0};
int num_modules = 0;
Msg *queue;
int queue_tail = 0;
int queue_head = 0;

void send(Msg msg);

int empty()
{
    return queue_tail == queue_head;
}

void enqueue(Msg msg)
{
    queue[queue_tail] = msg;
    queue_tail = (queue_tail + 1) % QUEUE_SIZE;
}

Msg pop()
{
    Msg ret = queue[queue_head];
    queue_head = (queue_head + 1) % QUEUE_SIZE;
    return ret;
}

int find(Module* modules, int num_modules, char *name)
{
    for (int i = 0; i < num_modules; i++)
    {
        if (strcmp(modules[i].name, name) == 0)
            return i;
    }

    return -1;
}

void broadcast(int idx, int pulse)
{
    for (int i = 0; i < num_modules; i++)
    {
        if (mat[idx][i] == 1)
        {
            enqueue((Msg){idx, i, pulse});
        }
    }
}

void send(Msg msg)
{
    sent[msg.src][msg.dest] = msg.pulse;
    count[msg.pulse]++;

    switch(modules[msg.dest].prefix)
    {
        case '%':
            if (msg.pulse == LOW)
            {
                modules[msg.dest].state = !modules[msg.dest].state;
                broadcast(msg.dest, modules[msg.dest].state);
            }
            break;
        case '&':
            {
                int result = LOW;
                for (int i = 0; i < num_modules; i++)
                {

                    if (mat[i][msg.dest] == 1 && sent[i][msg.dest] == LOW)
                    {
                        result = HIGH;
                        break;
                    }
                }
                broadcast(msg.dest, result);
            }
            break;
        default:
            broadcast(msg.dest, msg.pulse);
    }
}

int main(int argc, char* argv[])
{
    memset(mat, 0, sizeof(mat));
    memset(sent, 0, sizeof(sent));
    queue = malloc(sizeof(Msg) * QUEUE_SIZE);

    int broadcaster_idx;

    FILE *f = fopen(argv[1], "r");
    while (!feof(f))
    {
        char buf[1024];
        fgets(buf, sizeof(buf), f);

        regex_t regex;
        int reti = regcomp(&regex, "([%&a-z]*) -> (.*)", REG_EXTENDED);
        if (!reti)
        {
            regmatch_t matches[3];
            regexec(&regex, buf, sizeof(matches) / sizeof(matches[0]), matches, 0);

            int offset = 0;
            char prefix;
            char name[16];
            if (buf[0] == '%' || buf[0] == '&')
            {
                prefix = buf[0];
                offset = 1;
            }
            else
                prefix = 0;
            strncpy(name, buf + matches[1].rm_so + offset, matches[1].rm_eo - matches[1].rm_so - offset);
            name[matches[1].rm_eo - matches[1].rm_so - offset] = '\0';
            int idx = find(modules, num_modules, name);
            if (idx == -1)
            {
                idx = num_modules;
                num_modules++;
            }
            strcpy(modules[idx].name, name);
            modules[idx].prefix = prefix;
            modules[idx].state = OFF;

            char *tok = strtok(buf + matches[2].rm_so, " ,\n");
            while(tok)
            {
                int target_idx = find(modules, num_modules, tok);
                if (target_idx == -1)
                {
                    target_idx = num_modules;
                    strcpy(modules[target_idx].name, tok);
                    num_modules++;
                }
                mat[idx][target_idx] = 1;
                
                tok = strtok(NULL, " ,\n");
            }

            if (strcmp(modules[idx].name, "broadcaster") == 0)
                broadcaster_idx = idx;
        }
        regfree(&regex);
    }

    for (int i = 0; i < 1000; i++)
    {
        broadcast(broadcaster_idx, 0);
        count[LOW]++;
        while(!empty())
        {
            Msg msg = pop();
            //printf("sender/receiver/val: %d %d %d\n", msg.src, msg.dest, msg.pulse);
            send(msg);
        }
    }
    printf("%ld\n", count[0] * count[1]);
}