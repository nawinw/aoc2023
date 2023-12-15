#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <omp.h>

typedef struct
{
    int64_t *data;
    int64_t *dims;
    int64_t n_dims;
} Mat;

int64_t* at(Mat *mat, int64_t *indices)
{
    int64_t *val = mat->data;
    int64_t stride = 1;
    for (int64_t i = 0; i < mat->n_dims; i++)
    {
        if (indices[i] < 0 || indices[i] >= mat->dims[i])
        {
            printf("ERROR\n");
            exit(-1);
        }
        val += indices[i] * stride;
        stride *= mat->dims[i];
    }
    return val; 
}

Mat* make_mat3(int64_t rows, int64_t cols, int64_t channels, int64_t default_val)
{
    Mat* ret = (Mat*)malloc(sizeof(Mat));
    ret->n_dims = 3;
    ret->dims = (int64_t*)malloc(sizeof(ret->dims[0]) * ret->n_dims);
    ret->dims[0] = cols;
    ret->dims[1] = rows;
    ret->dims[2] = channels;
    ret->data = (int64_t*)malloc(sizeof(ret->data[0]) * cols * rows * channels);

    for (int64_t i = 0; i < cols * rows * channels; i++)
        ret->data[i] = default_val;

    return ret;
}

void delete_mat(Mat* mat)
{
    free(mat->data);
    free(mat->dims);
    free(mat);
}

int64_t count_arrangements(char *str, int64_t len, int64_t *record, int64_t record_len, int64_t count, int64_t idx, int64_t record_idx, Mat* memo)
{
    if (idx == len)
    {
        if (count == 0)
            return record_idx == record_len;
        else
            return record_idx == record_len -1 && record[record_idx] == count;
    }

    int64_t* memo_val = at(memo, (int64_t[]){count, idx, record_idx});
    if (*memo_val != -1)
        return *memo_val;

    int64_t val = 0;
    int64_t record_match = record_idx < record_len && record[record_idx] == count;
    switch (str[idx])
    {
        case '?':
            val += count_arrangements(str, len, record, record_len, count + 1, idx + 1, record_idx, memo);
        case '.':
            if (count == 0 || record_match)
                val += count_arrangements(str, len, record, record_len, 0, idx + 1, record_idx + record_match, memo);
            break;
        case '#':
            val += count_arrangements(str, len, record, record_len, count + 1, idx + 1, record_idx, memo);
            break;
        default:
            printf("ERROR: %c\n", str[idx]);
            exit(-1);
    }
    *memo_val = val;
    return val;
}

int main(int argc, char* argv[])
{
    FILE *f = fopen(argv[1], "r");
    char buf[1024];
    int64_t record[1024];
    int64_t record_len;

    int64_t total = 0;
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

#if 0
        *sep = '\0';
        int64_t len = strlen(buf);
#else
        *sep = '?';
        int64_t len = sep - buf + 1;

        for (int64_t i = 0; i < 4; i++)
        {
            memcpy(record + record_len * (i + 1), record, sizeof(int64_t) * record_len);
            memcpy(buf + len * (i + 1), buf, len);
        }
        len = len * 5 - 1;
        buf[len] = '\0';
        record_len *= 5;
#endif
        Mat* memo = make_mat3(len, len, record_len + 1, -1);

        int64_t count = count_arrangements(buf, len, record, record_len, 0, 0, 0, memo);
        total += count;
        printf("%ld\n", count);

        delete_mat(memo);
    }
    printf("%ld\n", total);
}

