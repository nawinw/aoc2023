#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

void main()
{
    // uint64_t time[] = {7, 15, 30};
    // uint64_t dist[] = {9, 40, 200};
    // uint64_t time[] = {58, 81, 96, 76};
    // uint64_t dist[] = {434, 1041, 2219, 1218};
    //uint64_t time[] = {71530};
    //uint64_t dist[] = {940200};
    uint64_t time[] = {58819676};
    uint64_t dist[] = {434104122191218};
    uint64_t count = sizeof(time) / sizeof(time[0]);
    uint64_t total = 1;
    for (uint64_t i = 0; i < count; i++)
    {
        uint64_t count = 0;
        for (uint64_t j = 1; j < time[i]; j++)
        {
            uint64_t remain = time[i] - j;
            uint64_t d = remain * j;
            if (d > dist[i])
                count++;
        }
        total *= count;
    }
    printf("%d\n", total);
}