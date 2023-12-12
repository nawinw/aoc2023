#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int next(int *nums, int len)
{
    int diffs[21];
    int zero = 1;
    for (int i = 0; i < len - 1; i++)
    {
        diffs[i] = nums[i + 1] - nums[i];
        if (diffs[i] != 0)
            zero = 0;
    }
    if (zero)
        return 0;
    
    return diffs[0] - next(diffs, len - 1);
}

int main(int argc, char* argv[])
{
    int n = 6;
    if (strcmp(argv[1], "data.dat") == 0)
        n = 21;
        
    FILE *f = fopen(argv[1], "r");
    int total = 0;
    while (!feof(f))
    {
        int nums[21];
        char buf[1024];
        for (int i = 0; i < n; i++)
            fscanf(f, "%d", &(nums[i]));

        total += nums[0] - next(nums, n);
    }
    printf("%d\n", total);
}