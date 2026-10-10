
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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
        perror("File open error");
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

void printReverse(void)
{
    for (int i = lineCount - 1; i >= 0; i--) {
        printf("%s\n", savedText[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    readFile(argv[1]);

    printReverse();

    return 0;
}

