#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main(int argc, char *argv[])
{
    struct student rec;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "How to use: %s FileName\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "w");

    if (fp == NULL) {
        fprintf(stderr, "File Open Error\n");
        return 2;
    }

    while (scanf("%d %19s %hd",
                 &rec.id, rec.name, &rec.score) == 3) {

        fprintf(fp, "%d %s %hd\n",
                rec.id, rec.name, rec.score);
    }

    fclose(fp);

    return 0;
}
