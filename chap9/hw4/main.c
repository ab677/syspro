
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARG 100

int main() {
    char line[MAXLINE];
    char *args[MAXARG];

    while (1) {
        printf("[Shell] ");
        fflush(stdout);

        if (fgets(line, MAXLINE, stdin) == NULL)
            break;

        int len = strlen(line);

        while (len > 0 &&
               (line[len - 1] == '\n' || line[len - 1] == ' ' ||
                line[len - 1] == '\t')) {
            line[--len] = '\0';
        }

        if (len == 0)
            continue;

        if (strcmp(line, "exit") == 0)
            break;

        int background = 0;

        if (line[len - 1] == '&') {
            background = 1;
            line[--len] = '\0';
        }

        char *saveptr;
        char *token;
        int argc = 0;

        token = strtok_r(line, " \t\n", &saveptr);

        while (token != NULL && argc < MAXARG - 1) {
            args[argc++] = token;
            token = strtok_r(NULL, " \t\n", &saveptr);
        }

        args[argc] = NULL;

        if (argc == 0)
            continue;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execvp(args[0], args);

            perror("execvp");
            exit(1);
        }
        else {
            if (background) {
                printf("[Background] PID: %d\n", pid);
            }
            else {
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
}

