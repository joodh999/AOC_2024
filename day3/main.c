#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 12
int sum = 0;
int docond = 1;

void clearbuf(char *buffer, int *buf_idx)
{
    memset(buffer, 0, BUFFER_SIZE);
    (*buf_idx) = -1;
}

int mul(char *buffer, int *buf_index)
{
    const char *muldef = "mul(";
    const char *dodef = "do()";
    const char *dontdef = "don't()";

    const int muldeflen = strlen(muldef) - 1;

    int a, b = 0;

    if (*buf_index > muldeflen && strncmp(buffer, muldef, muldeflen) == 0)
    {

        char *comma = strchr(buffer, ',');
        char *end = strchr(buffer, ')');

        if (end)
        {
            if ((sscanf(buffer, "mul(%d,%d)", &a, &b) == 2) && docond)
            {
                printf("%d %d\n", a, b);
                sum += (a * b);
            }

            clearbuf(buffer, buf_index);
        }
        else
        {
            if (!comma && !isdigit(buffer[*buf_index]))
                clearbuf(buffer, buf_index);
            // comma and not a digit given that buf_index is NOT ','
            else if (comma && !isdigit(buffer[*buf_index]) && (buffer[*buf_index] != ','))
            {
                clearbuf(buffer, buf_index);
            }
        }
    }
    // part 2
    else if (buffer[*buf_index] == dodef[*buf_index] || buffer[*buf_index] == dontdef[*buf_index])
    {
        if (strncmp(dontdef, buffer, strlen(dontdef)) == 0)
        {
            docond = 0;
            clearbuf(buffer, buf_index);
        }
        if (strncmp(dodef, buffer, strlen(dodef)) == 0)
        {

            docond = 1;
            clearbuf(buffer, buf_index);
        }
    }

    else if (buffer[*buf_index] != muldef[*buf_index])
    {
        clearbuf(buffer, buf_index);
    }

    if (*buf_index >= BUFFER_SIZE - 1)
    {
        clearbuf(buffer, buf_index);
    }

    return 0;
}

int main()
{
    FILE *input = fopen("input.txt", "r");
    if (!input)
    {
        printf("Error opening file\n");
        return -1;
    }

    int ch;
    char buffer[BUFFER_SIZE] = {0};
    int buffer_idx = -1;

    while ((ch = getc(input)) != EOF)
    {
        buffer[++buffer_idx] = ch;
        mul(buffer, &buffer_idx);
    }

    fclose(input);
    printf("\n Sum: %d\n", sum);
    return 0;
}

/*
string : mul(


*/