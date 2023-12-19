#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include <regex.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct
{
    char var;
    char cmp;
    int val;
    char target[8];
} Rule;

typedef struct
{
    char name[8];
    Rule rule[8];
    int num_rules;
} Workflow;

int find(Workflow* workflows, int num_workflows, char* name)
{
    for (int i = 0; i < num_workflows; i++)
    {
        if (strcmp(workflows[i].name, name) == 0)
            return i;
    }

    printf("ERROR\n");
    exit(-1);
}

int64_t count_ratings(Workflow* workflows, int num_workflows, char *name, int x0, int x1, int m0, int m1, int a0, int a1, int s0, int s1)
{
    printf("%s\n", name);
    if (x0 >= x1 || m0 >= m1 || a0 >= a1 || s0 >= s1)
        return 0;
    else if (name[0] == 'A')
    {
        int64_t ret = ((int64_t)x1 - x0 + 1) * (m1 - m0 + 1) * (a1 - a0 + 1) * (s1 - s0 + 1);
        printf("%ld\n", ret);
        return ret;
    }
    else if (name[0] == 'R')
        return 0;

    int idx = find(workflows, num_workflows, name);

    int64_t total = 0;
    for (int i = 0; i < workflows[idx].num_rules; i++)
    {
        if (x0 >= x1 || m0 >= m1 || a0 >= a1 || s0 >= s1)
            return total;

        if (workflows[idx].rule[i].var == 0)
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, m1, a0, a1, s0, s1);
        else if (workflows[idx].rule[i].var == 'x' && workflows[idx].rule[i].cmp == '>')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, MAX(workflows[idx].rule[i].val + 1, x0), x1, m0, m1, a0, a1, s0, s1);
            x1 = MIN(workflows[idx].rule[i].val, x1);
        }
        else if (workflows[idx].rule[i].var == 'x' && workflows[idx].rule[i].cmp == '<')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, MIN(workflows[idx].rule[i].val - 1, x1), m0, m1, a0, a1, s0, s1);
            x0 = MAX(workflows[idx].rule[i].val, x0);
        }
        else if (workflows[idx].rule[i].var == 'm' && workflows[idx].rule[i].cmp == '>')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, MAX(workflows[idx].rule[i].val + 1, m0), m1, a0, a1, s0, s1);
            m1 = MIN(workflows[idx].rule[i].val, m1);
        }
        else if (workflows[idx].rule[i].var == 'm' && workflows[idx].rule[i].cmp == '<')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, MIN(workflows[idx].rule[i].val - 1, m1), a0, a1, s0, s1);
            m0 = MAX(workflows[idx].rule[i].val, m0);
        }
        else if (workflows[idx].rule[i].var == 'a' && workflows[idx].rule[i].cmp == '>')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, m1, MAX(workflows[idx].rule[i].val + 1, a0), a1, s0, s1);
            a1 = MIN(workflows[idx].rule[i].val, a1);
        }
        else if (workflows[idx].rule[i].var == 'a' && workflows[idx].rule[i].cmp == '<')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, m1, a0, MIN(workflows[idx].rule[i].val - 1, a1), s0, s1);
            a0 = MAX(workflows[idx].rule[i].val, a0);
        }
        else if (workflows[idx].rule[i].var == 's' && workflows[idx].rule[i].cmp == '>')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, m1, a0, a1, MAX(workflows[idx].rule[i].val + 1, s0), s1);
            s1 = MIN(workflows[idx].rule[i].val, s1);
        }
        else if (workflows[idx].rule[i].var == 's' && workflows[idx].rule[i].cmp == '<')
        {
            total += count_ratings(workflows, num_workflows, workflows[idx].rule[i].target, x0, x1, m0, m1, a0, a1, s0, MIN(workflows[idx].rule[i].val - 1, s1));
            s0 = MAX(workflows[idx].rule[i].val, s0);
        }
        else
        {
            printf("ERROR\n");
            exit(-1);
        }
    }
    return total;
}

int main(int argc, char* argv[])
{
    char a2i[256];
    a2i['x'] = 0;
    a2i['m'] = 1;
    a2i['a'] = 2;
    a2i['s'] = 3;

    Workflow* workflows = malloc(sizeof(Workflow) * 1024);
    int num_workflows = 0;

    FILE *f = fopen(argv[1], "r");
    while (!feof(f))
    {
        char buf[1024];
        fgets(buf, sizeof(buf), f);

        if (buf[0] == '{')
            continue;
        else if (buf[0] != '\n')
        {
            Workflow* workflow = workflows + num_workflows;

            regex_t regex;
            int reti = regcomp(&regex, "([a-z]*)\\{(.*)\\}", REG_EXTENDED);
            if (!reti)
            {
                regmatch_t matches[8];
                regexec(&regex, buf, 8, matches, 0);
                strncpy(workflow->name, buf + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);

                char *tok = strtok(buf + matches[2].rm_so, ",}\n");
                while(tok)
                {
                    if (tok[1] == '<' || tok[1] == '>')
                    {
                        workflow->rule[workflow->num_rules] = (Rule){tok[0], tok[1]};
                        char *sep = strstr(tok, ":");
                        *sep = '\0';
                        workflow->rule[workflow->num_rules].val = atoi(tok + 2);
                        strcpy(workflow->rule[workflow->num_rules++].target, sep + 1);
                    }
                    else
                    {
                        workflow->rule[workflow->num_rules] = (Rule){0, 0};
                        strcpy(workflow->rule[workflow->num_rules++].target, tok);
                    }
                    tok = strtok(NULL, ",}\n");
                }
            }
            regfree(&regex);

            num_workflows++;
        }
    }

    int64_t total = count_ratings(workflows, num_workflows, "in", 1, 4000, 1, 4000, 1, 4000, 1, 4000);
    printf("%ld\n", total);
}