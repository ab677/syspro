
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        printf("Usage: %s [-e [ENV_NAME]] [-u] [-g] [-i] [-p]\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-e") == 0) {

            if (i + 1 < argc && argv[i + 1][0] != '-') {
                char *value = getenv(argv[i + 1]);

                if (value != NULL)
                    printf("%s = %s\n", argv[i + 1], value);
                else
                    printf("%s: Environment variable not found\n", argv[i + 1]);

                i++;
            }
            else {
                int j = 0;

                while (environ[j] != NULL) {
                    printf("%s\n", environ[j]);
                    j++;
                }
            }
        }

        else if (strcmp(argv[i], "-u") == 0) {
            printf("Real User ID: %d\n", getuid());
            printf("Effective User ID: %d\n", geteuid());
        }

        else if (strcmp(argv[i], "-g") == 0) {
            printf("Real Group ID: %d\n", getgid());
            printf("Effective Group ID: %d\n", getegid());
        }

        else if (strcmp(argv[i], "-i") == 0) {
            printf("Process ID: %d\n", getpid());
        }

        else if (strcmp(argv[i], "-p") == 0) {
            printf("Parent Process ID: %d\n", getppid());
        }

        else {
            printf("Unknown option: %s\n", argv[i]);
        }
    }

    return 0;
}

