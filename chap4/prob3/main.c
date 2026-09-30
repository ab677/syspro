#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 80

int main(int argc, char *argv[]){
    FILE *fp;
    int line = 0;
    char buffer[MAXLINE];
    if (argc != 2){
        fprintf(stderr, "How to use : %s FileName\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "r");

    if (fp == NULL){
        fprintf(stderr, "Error Open File\n");
        return 2;
    }
    while (fgets(buffer, MAXLINE, fp) != NULL){
        line++;
        printf("%3d %s", line, buffer);
    }
    fclose(fp);
    return 0;
}
