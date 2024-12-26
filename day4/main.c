#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int rowc, colc = 0;
char **puzzle;

char **
getpuzzle(FILE *input)
{
    char **puzzle = NULL;
    char *row = NULL;

    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&row, &len, input)) != -1)
    {

        if (row[nread - 1] == '\n')
            nread--;

        if (colc == 0)
            colc = nread;
        else
            assert(nread == colc);

        // Allocate memory for the new row
        char **tmppuzzle = realloc(puzzle, (rowc + 1) * sizeof(char *));
        if (!tmppuzzle)
        {
            fprintf(stderr, "failed to allocate mem for tmppuzzle\n");
            return NULL;
        }
        puzzle = tmppuzzle;

        puzzle[rowc] = malloc((colc + 1) * sizeof(char));
        if (!puzzle[rowc])
        {
            fprintf(stderr, "failed to allocate mem for row\n");
            return NULL;
        }

        // strcpy(puzzle[rowc], row);
        strncpy(puzzle[rowc], row, nread);

        rowc++;
    }

    free(row);
    fclose(input);

    return puzzle;
}

// A & S
bool match(int i, int j, int y_off, int x_off, char chr)
{
    int offsets[] = {1, 0, -1};

    int X = i + offsets[x_off];
    int Y = j + offsets[y_off]; // transformed coordinates

    if (!(X >= 0 && X < colc && Y >= 0 && Y < rowc))
        return false;

    if (puzzle[Y][X] == chr)
    {
        if (chr == 'M')
        {
            return match(X, Y, y_off, x_off, 'A');
        }
        else if (chr == 'A')
        {
            return match(X, Y, y_off, x_off, 'S');
        }
        else if (puzzle[Y][X] == 'S')
            return true;
    }

    return false;
}

int main()
{
    FILE *file = fopen("input.txt", "r");
    assert(file);

    puzzle = getpuzzle(file);
    assert(puzzle);

    int count = 0;

    // Print the puzzle

    for (size_t j = 0; j < rowc; j++)
    {
        for (size_t i = 0; i < colc; i++)
        {
            if (puzzle[j][i] == 'X')
            {

                for (int y_offset = 0; y_offset <= 2; y_offset++)
                {
                    for (int x_offset = 0; x_offset <= 2; x_offset++)
                    {

                        if (match(i, j, y_offset, x_offset, 'M'))
                        {
                            count++;
                            printf("i:%d j:%d \n", i, j);
                        }
                    }
                }
            }
        }
    }
    printf("\n");

    for (size_t i = 0; i < rowc; i++)
    {
        printf("%s\n", puzzle[i]);
    }
    printf("\n");

    printf("count %d\n", count);
    return 0;
}
