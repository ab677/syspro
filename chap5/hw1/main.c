
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LINES 10
#define MAX_LENGTH 100

char savedText[MAX_LINES][MAX_LENGTH];
int lineCount = 0;

void readFile(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    char buf;
    int col = 0;
    ssize_t n;

    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    while ((n = read(fd, &buf, 1)) > 0) {
        if (buf == '\n') {
            if (col > 0 && savedText[lineCount][col - 1] == '\r')
                col--;

            savedText[lineCount][col] = '\0';
            lineCount++;
            col = 0;

            if (lineCount >= MAX_LINES)
                break;
        }
        else if (col < MAX_LENGTH - 1) {
            savedText[lineCount][col++] = buf;
        }
    }

    if (n >= 0 && lineCount < MAX_LINES && col > 0) {
        if (savedText[lineCount][col - 1] == '\r')
            col--;

        savedText[lineCount][col] = '\0';
        lineCount++;
    }

    close(fd);
}

char *trim(char *str)
{
    while (isspace((unsigned char)*str))
        str++;

    char *end = str + strlen(str);

    while (end > str && isspace((unsigned char)end[-1]))
        end--;

    *end = '\0';
    return str;
}

void printLine(int num)
{
    if (num >= 1 && num <= lineCount)
        printf("%d: %s\n", num, savedText[num - 1]);
    else
        printf("Line %d does not exist.\n", num);
}

void printSelectedLines(char *input)
{
    char *token;

    input = trim(input);

    if (strcmp(input, "*") == 0) {
        for (int i = 1; i <= lineCount; i++)
            printLine(i);
        return;
    }

    token = strtok(input, ",");

    while (token != NULL) {
        token = trim(token);

        // Process a range: n-m
        char *dash = strchr(token, '-');

        if (dash != NULL) {
            *dash = '\0';

            char *startStr = trim(token);
            char *endStr = trim(dash + 1);
            char *p1, *p2;

            long start = strtol(startStr, &p1, 10);
            long end = strtol(endStr, &p2, 10);

            if (*startStr != '\0' && *endStr != '\0' &&
                *trim(p1) == '\0' && *trim(p2) == '\0' &&
                start >= 1 && end >= start &&
                end <= lineCount) {

                for (int i = (int)start; i <= (int)end; i++)
                    printLine(i);
            }
            else {
                printf("Invalid line range.\n");
            }
        }
        else {
            char *p;
            long num = strtol(token, &p, 10);

            if (*token != '\0' && *trim(p) == '\0' && num >= 1) {
                printLine((int)num);
            }
            else {
                printf("Invalid line number.\n");
            }
        }

        token = strtok(NULL, ",");
    }
}


int main(int argc, char *argv[])
{
    char input[256];

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    readFile(argv[1]);

    printf("Total Lines: %d\n", lineCount);

    while (1) {
        printf("\nEnter line number(s) (* for all, q to quit): ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(trim(input), "q") == 0)
            break;

        printSelectedLines(input);
    }

    printf("Program terminated.\n");

    return 0;
}

