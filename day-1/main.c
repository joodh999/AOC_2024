#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"

int size = 0;

int append(int **arr, int value)
{

    int *tmparr = realloc(*arr, (size + 1) * sizeof(int));
    if (tmparr == NULL)
    {
        printf("mem (re)allocation failed");
        return 1;
    }
    *arr = tmparr;
    (*arr)[size] = value;
    return 0;
}

int intcmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main()
{

    int *col1 = malloc(sizeof(int));
    int *col2 = malloc(sizeof(int));

    int tmp, tmp2 = 0;
    int totaldist = 0;
    int dist;

    int simiscore = 0;
    int freq = 0;

    FILE *input = fopen(INPUT_FILE, "r");
    if (input == NULL)
    {
        printf("failed to open file");
        return 1;
    }

    while (fscanf(input, "%d %d", &tmp, &tmp2) == 2)
    {
        if (append(&col1, tmp) != 0 || append(&col2, tmp2) != 0)
        {
            printf("Error appending values\n");
            return 1;
        }
        size++;
    }

    qsort(&col1[0], size, sizeof(int), intcmp);
    qsort(&col2[0], size, sizeof(int), intcmp);

    for (int i = 0; i < size; i++)
    {
        totaldist = totaldist + abs(col1[i] - col2[i]);
    }

    for (int i = 0; i < size; i++)
    {
        freq = 0;
        for (int j = 0; j < size; j++)
        {
            if (col1[i] == col2[j])
                freq++;
        }
        simiscore = simiscore + ((col1[i] * freq));
    }

    printf("similirity %d \n", simiscore);
    printf("%d", totaldist);

    fclose(input);
    free(col1);
    free(col2);
}
