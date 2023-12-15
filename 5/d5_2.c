#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int64_t main(int64_t argc, char* argv[])
{
    uint64_t seeds[2048];
    size_t count = 0;

    FILE *f = fopen(argv[1], "r");
    char buf[1024];
    fgets(buf, sizeof(buf), f);
    printf("%s", buf);
    char *tok = strtok(buf, " ");
    while(tok)
    {
        if (isdigit(tok[0]))
        {
            seeds[count++] = atoi(tok);
        }
        tok = strtok(NULL, " ");
    }

    uint64_t dst_start[256];
    uint64_t src_start[256];
    uint64_t length[256];
    uint64_t num_rules = 0;
    while (!feof(f))
    {

        fgets(buf, sizeof(buf), f);
        if (isdigit(buf[0]))
        {
            sscanf(buf, "%lu %lu %lu", dst_start + num_rules, src_start + num_rules, length + num_rules);
            //printf("%lu %lu %lu\n", dst_start[num_rules], src_start[num_rules], length[num_rules]);
            num_rules++;
        }
        else if (isalpha(buf[0]) || feof(f))
        {
            //printf("-------------\n");
            for (int i = 0; i < count; i+=2)
            {
                for (int j = 0; j < num_rules; j++)
                {
                    uint64_t seed_start = seeds[i];
                    uint64_t seed_end = seeds[i] + seeds[i + 1];
                    uint64_t rule_start = src_start[j];
                    uint64_t rule_end = src_start[j] + length[j];

                    if (seed_start < rule_end && rule_start < seed_end)
                    {
                        // Case 1: Seed range contained in rule
                        if (seed_start >= rule_start && seed_end <= rule_end)
                        {
                            // Transform seed
                            seeds[i] = dst_start[j] + seed_start - rule_start;
                            break;
                        }
                        // Case 2: Rule starts inside of seed range
                        // Truncate the unaffected first part, and leave the remainder to be re-processed as a new segment
                        else if (rule_start > seed_start)
                        {
                            // Truncate seed
                            seeds[i + 1] = rule_start - seed_start;

                            // Create new seed from remainder
                            seeds[count++] = rule_start;
                            seeds[count++] = seed_end - rule_start;
                        }
                        // Case 3: Rule starts before or at seed range
                        // Transform the first part of the seed range, and leave the rest to be re-processed
                        else
                        {
                            // Transform and truncate seed
                            seeds[i] = dst_start[j] + seed_start - rule_start;
                            uint64_t end = MIN(rule_end, seed_end);
                            seeds[i + 1] = end - seed_start;

                            // Create new seed from remainder
                            seeds[count++] = end;
                            seeds[count++] = MAX(rule_end, seed_end) - end;
                            break;
                        }
                    }
                }
            }
            num_rules = 0;
        }
    }
    uint64_t lowest = seeds[0];
    for (int i = 2; i < count; i+=2)
    {
        if (seeds[i] < lowest)
            lowest = seeds[i];
    }
    printf("%lu\n", lowest);
}

