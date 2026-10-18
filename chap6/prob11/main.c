#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

char type(mode_t);
char *perm(mode_t);
void printStat(char *, char *, struct stat *, int, int, int);

int main(int argc, char **argv) {
    DIR *dp;
    char *dir;
    struct stat st;
    struct dirent *d;
    char path[BUFSIZ + 1];

    int opt;
    int option_i = 0;
    int option_p = 0;
    int option_Q = 0;

    while ((opt = getopt(argc, argv, "ipQ")) != -1) {
        switch (opt) {
            case 'i':
                option_i = 1;
                break;

            case 'p':
                option_p = 1;
                break;

            case 'Q':
                option_Q = 1;
                break;

            default:
                printf("Usage: %s [-i] [-p] [-Q] [directory]\n", argv[0]);
                exit(1);
        }
    }

    if (optind < argc)
        dir = argv[optind];
    else
        dir = ".";

    if ((dp = opendir(dir)) == NULL) {
        perror("opendir error");
        exit(1);
    }

    while ((d = readdir(dp)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);

        if (lstat(path, &st) < 0) {
            perror("lstat error");
            continue;
        }

        printStat(path, d->d_name, &st,
                  option_i, option_p, option_Q);
    }

    closedir(dp);

    return 0;
}

void printStat(char *pathname, char *file, struct stat *st,
               int option_i, int option_p, int option_Q) {

    if (option_i) {
        printf("%-10lu ",
               (unsigned long)st->st_ino);
    }

    printf("%5ld ", (long)st->st_blocks);

    printf("%c%s ",
           type(st->st_mode),
           perm(st->st_mode));

    printf("%3lu ",
           (unsigned long)st->st_nlink);

    struct passwd *pw = getpwuid(st->st_uid);
    struct group *gr = getgrgid(st->st_gid);

    printf("%s %s ",
           pw ? pw->pw_name : "unknown",
           gr ? gr->gr_name : "unknown");

    printf("%9ld ",
           (long)st->st_size);

    printf("%.12s ",
           ctime(&st->st_mtime) + 4);

    if (option_Q)
        printf("\"%s\"", file);
    else
        printf("%s", file);

    if (option_p && S_ISDIR(st->st_mode))
        printf("/");

    printf("\n");
}

char type(mode_t mode) {
    if (S_ISREG(mode))
        return '-';

    if (S_ISDIR(mode))
        return 'd';

    if (S_ISCHR(mode))
        return 'c';

    if (S_ISBLK(mode))
        return 'b';

    if (S_ISLNK(mode))
        return 'l';

    if (S_ISFIFO(mode))
        return 'p';

    if (S_ISSOCK(mode))
        return 's';

    return '?';
}

char* perm(mode_t mode) {
    static char perms[10];

    strcpy(perms, "---------");

    for (int i = 0; i < 3; i++) {
        if (mode & (S_IRUSR >> (i * 3)))
            perms[i * 3] = 'r';

        if (mode & (S_IWUSR >> (i * 3)))
            perms[i * 3 + 1] = 'w';

        if (mode & (S_IXUSR >> (i * 3)))
            perms[i * 3 + 2] = 'x';
    }

    return perms;
}
