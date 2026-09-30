#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _IO_UNBUFFERED 0x0002
#define _IO_LINE_BUF   0x0200

int main(int argc, char *argv[])
{
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "How to use: %s stdin|stdout|stderr|FileName\n",
                argv[0]);
        return 1;
    }

    if (!strcmp(argv[1], "stdin")) {
        fp = stdin;

        printf("Enter one letter: ");

        if (getchar() == EOF) {
            perror("getchar");
            return 2;
        }
    }
    else if (!strcmp(argv[1], "stdout")) {
        fp = stdout;
    }
    else if (!strcmp(argv[1], "stderr")) {
        fp = stderr;
    }
    else {
        fp = fopen(argv[1], "r");

        if (fp == NULL) {
            perror("fopen");
            return 3;
        }

        if (getc(fp) == EOF) {
            perror("getc");
        }
    }

    printf("Stream = %s, ", argv[1]);

    if (fp->_flags & _IO_UNBUFFERED)
        printf("Unbuffered");
    else if (fp->_flags & _IO_LINE_BUF)
        printf("Line buffered");
    else
        printf("Fully buffered");

    printf(", Buffer size = %ld\n",
           (long)(fp->_IO_buf_end - fp->_IO_buf_base));

    if (fp != stdin && fp != stdout && fp != stderr)
        fclose(fp);

    return 0;
}

