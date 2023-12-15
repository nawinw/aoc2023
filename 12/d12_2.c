#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <omp.h>

typedef struct
{
    int *data;
    int *dims;
    int n_dims;
} Mat;

int* at(Mat *mat, int *indices)
{
    int *val = mat->data;
    int stride = 1;
    for (int i = 0; i < mat->n_dims; i++)
    {
        val += indices[i] * stride;
        stride *= mat->dims[i];
    }
    return val; 
}

Mat* make_mat3(int rows, int cols, int channels, int default_val)
{
    Mat* ret = (Mat*)malloc(sizeof(Mat));
    ret->n_dims = 3;
    ret->dims = (int*)malloc(sizeof(ret->dims[0]) * ret->n_dims);
    ret->dims[0] = cols;
    ret->dims[1] = rows;
    ret->dims[2] = channels;

    for (int i = 0; i < cols * rows * channels; i++)
        ret->data[i] = default_val;

    return ret;
}

void delete_mat(Mat* mat)
{
    free(mat->data);
    free(mat->dims);
    free(mat);
}

int count_arrangements(char *str, int len, int *record, int record_len, int count, int idx, int record_idx, Mat* memo)
{
    int memo_val = *at(memo, (int[]){count, idx, record_idx});
    if (memo_val != -1)
        return memo_val;

    
}

int64_t main(int64_t argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    char buf[1024];
    int record[1024];
    int record_len;

    int total = 0;
    while (!feof(f))
    {
        record_len = 0;

        fgets(buf, sizeof(buf), f);
        char *sep = strstr(buf, " ");
        char *tok = strtok(sep + 1, ",");
        while (tok)
        {
            record[record_len] = atoi(tok);
            record_len++;
            tok = strtok(NULL, ",");
        }

#if 1
        *sep = '\0';
        int len = strlen(buf);
#else
        *sep = '?';
        int len = sep - buf + 1;

        for (int i = 0; i < 4; i++)
        {
            memcpy(record + record_len * (i + 1), record, sizeof(int) * record_len);
            memcpy(buf + len * (i + 1), buf, len);
        }
        buf[len * 5 - 1] = '\0';
        record_len *= 5;
#endif
        Mat* memo = make_mat3(len, len, record_len, -1);

        int count = count_arrangements(buf, len, record, record_len, 0, 0, 0, memo);
        total += count;
        printf("%d\n", count);

        delete_mat(memo);
    }
    printf("%d\n", total);
}

