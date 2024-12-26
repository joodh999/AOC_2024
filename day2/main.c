#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define DECREASING -1
#define INCREASING 1

// returns an int array
int *convert(char *report, int buffer_size, int *index)
{
    char *token = NULL;
    char *saveptr1;

    *index = 0;

    int *levels = malloc(buffer_size * sizeof(int));

    if (levels == NULL)
    {
        fprintf(stderr, "memory allocation failed (for levels buffer)\n");
        return NULL;
    }

    token = strtok_r(report, " ", &saveptr1);

    while (token != NULL)
    {
        // printf("%d\n", atoi(token));
        levels[*index] = atoi(token);
        (*index)++;
        token = strtok_r(NULL, " ", &saveptr1);
    }

    return levels;
}

int check_safe(int *report, int *index, int *abn_level)
{
    int diff = 0;
    int trend = 0;

    for (int i = 0; i < *(index)-1; i++)
    {
        diff = report[i + 1] - report[i];
        if (abs(diff) > 3 || abs(diff) == 0)
        {
            *abn_level = i;
            return 0;
        }
        else if (diff < 0)
        {
            if (trend == INCREASING)
            {
                *abn_level = i;
                return 0;
            }
            trend = DECREASING;
        }
        else if (diff > 0)
        {
            if (trend == DECREASING)
            {
                *abn_level = i;
                return 0;
            }
            trend = INCREASING;
        }
    }

    return 1;
}

// remove abnormal level from report; return a ptr to new report
int *Problem_Dampener(int *report, int abnormal, int index)
{

    int *tmp = malloc((index - 1) * sizeof(int));
    if (tmp == NULL)
        return NULL;

    for (int i = 0, j = 0; i < index; i++)
    {
        if (i != abnormal)
        {
            tmp[j] = report[i];
            j++;
        }
    }
    printf("New report after dampening: ");
    for (int i = 0; i < index - 1; i++)
    {
        printf("%d ", tmp[i]);
    }
    return tmp;
}

int main()
{
    FILE *input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }

    ssize_t nread;
    size_t len = 0;
    char *rawreport = NULL;

    int *report;
    int safe_count = 0;

    while ((nread = getline(&rawreport, &len, input)) != -1)
    {

        int index = 0;
        int ABN_levl = 0;
        int safe = -1;

        report = convert(rawreport, len, &index);
        if (report == NULL)
        {
            fprintf(stderr, "parsing failed\n");
            free(report);
            fclose(input);
            return 1;
        }

        if ((safe = check_safe(report, &index, &ABN_levl)))
        {
            safe_count++;
            free(report);
        }
        else
        { // when not safe
            int found_safe = 0;
            // Try removing each level one at a time
            for (int i = 0; i < index && !found_safe; i++)
            {
                int *newreport = Problem_Dampener(report, i, index);
                if (newreport == NULL)
                    continue;

                int newindex = index - 1;
                int new_abn = 0;
                if (check_safe(newreport, &newindex, &new_abn))
                {
                    safe_count++;
                    found_safe = 1;
                }
                free(newreport);
            }
            free(report);
        }
    }

    printf("safe %d\n", safe_count);
    fclose(input);
    return 0;
}
