#include <stdio.h>
#include <string.h>
#include "copy.h"

int main()
{
    char line[5][MAXLINE];
    char temp[MAXLINE];

    int i, j;

    for (i = 0; i < 5; i++)
    {
        fgets(line[i], MAXLINE, stdin);

        line[i][strcspn(line[i], "\n")] = '\0';
    }

    for (i = 0; i < 4; i++)
    {
        for (j = i + 1; j < 5; j++)
        {
            if (strlen(line[i]) < strlen(line[j]))
            {
                copy(line[i], temp);
                copy(line[j], line[i]);
                copy(temp, line[j]);
            }
        }
    }

    for (i = 0; i < 5; i++)
    {
        printf("%s \n", line[i], (int)strlen(line[i]));
    }

    return 0;
}
