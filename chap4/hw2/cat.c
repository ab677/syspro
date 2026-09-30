#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int c;
    int line = 1;
    int n_option = 0;
    int start = 1;
    int i;

    if (argc < 2) {
        fprintf(stderr, "How to use: %s [-n] File1 [File2 ...]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0) {
        n_option = 1;
        start = 2;

        if (argc < 3) {
            fprintf(stderr, "How to use: %s [-n] File1 [File2 ...]\n", argv[0]);
            return 1;
        }
    }

    for (i = start; i < argc; i++) {

        fp = fopen(argv[i], "r");

        if (fp == NULL) {
            fprintf(stderr, "Error Open File: %s\n", argv[i]);
            continue;
        }

        while ((c = fgetc(fp)) != EOF) {

            if (n_option && line == 1)
                printf("%6d\t", line);

            putchar(c);

            if (c == '\n') {
                line++;

                if (n_option && i + 1 < argc)
                    printf("%6d\t", line);
            }
        }

        fclose(fp);
    }

    return 0;
}

