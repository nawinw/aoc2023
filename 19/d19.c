#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <sys/types.h>
#include <regex.h>

typedef struct
{
    int n[4];
    char workflow[8];
} Part;

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


int main(int argc, char* argv[])
{
    char a2i[256];
    a2i['x'] = 0;
    a2i['m'] = 1;
    a2i['a'] = 2;
    a2i['s'] = 3;

    Part* parts = malloc(sizeof(Part) * 512);
    int num_parts = 0;

    Workflow* workflows = malloc(sizeof(Workflow) * 1024);
    int num_workflows = 0;

    FILE *f = fopen(argv[1], "r");
    while (!feof(f))
    {
        char buf[1024];
        fgets(buf, sizeof(buf), f);

        // Parts part
        if (buf[0] == '{')
        {
            for (int i = 0; buf[i] != '\0'; i++)
            {
                if (!isdigit(buf[i]))
                    buf[i] = ' ';
            }
            char *str = strtok(buf, " ");
            int i = 0;
            while(str)
            {
                parts[num_parts].n[i++] = atoi(str);
                str = strtok(NULL, " ");
            }
            //printf("%d %d %d %d\n", parts[num_parts].n[0], parts[num_parts].n[1], parts[num_parts].n[2], parts[num_parts].n[3]);
            num_parts++;
        }
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

    int total = 0;
    for (int i = 0; i < num_parts; i++)
    {
        strcpy(parts[i].workflow, "in");
        while(1)
        {
            for (int j = 0; j < num_workflows; j++)
            {
                if (strcmp(parts[i].workflow, workflows[j].name) == 0)
                {
                    for (int k = 0; k < workflows[j].num_rules; k++)
                    {
                        Rule *rule = &(workflows[j].rule[k]);
                        if (    rule->var == 0 || 
                                (rule->cmp == '<' && parts[i].n[a2i[rule->var]] < rule->val) ||
                                (rule->cmp == '>' && parts[i].n[a2i[rule->var]] > rule->val)
                                )
                        {
                            strcpy(parts[i].workflow, rule->target);
                            //printf("%s\n", rule->target);
                            break;
                        }
                    }
                }
            }
            if (parts[i].workflow[0] == 'A')
            {
                total += parts[i].n[0] + parts[i].n[1] + parts[i].n[2] + parts[i].n[3];
                break;
            }
            else if (parts[i].workflow[0] == 'R')
                break;
        }
    }
    printf("%d\n", total);
}