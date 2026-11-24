
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

void trim(char *str) {
    int len = strlen(str);

    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }

    int start = 0;

    while (str[start] && isspace((unsigned char)str[start])) {
        start++;
    }

    if (start > 0) {
        memmove(str, str + start, strlen(str + start) + 1);
    }
}

void execute_command(char *cmd) {
    char *args[MAX_ARGS];
    char *input_file = NULL;
    char *output_file = NULL;

    int background = 0;
    int argc = 0;

    trim(cmd);

    if (strlen(cmd) == 0)
        return;

    int len = strlen(cmd);

    if (cmd[len - 1] == '&') {
        background = 1;
        cmd[len - 1] = '\0';
        trim(cmd);
    }

    char *token = strtok(cmd, " \t\n");

    while (token != NULL && argc < MAX_ARGS - 1) {

        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");

            if (token == NULL) {
                fprintf(stderr, "Input file missing\n");
                return;
            }

            input_file = token;
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");

            if (token == NULL) {
                fprintf(stderr, "Output file missing\n");
                return;
            }

            output_file = token;
        }
        else {
            args[argc++] = token;
        }

        token = strtok(NULL, " \t\n");
    }

    args[argc] = NULL;

    if (argc == 0)
        return;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {

        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);

            if (fd < 0) {
                perror("open input");
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2 input");
                close(fd);
                exit(1);
            }

            close(fd);
        }

        if (output_file != NULL) {
            int fd = open(output_file,
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0) {
                perror("open output");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2 output");
                close(fd);
                exit(1);
            }

            close(fd);
        }

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

int main() {
    char line[MAX_LINE];

    while (1) {
        printf("[shell] ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        char *command;
        char *saveptr;

        command = strtok_r(line, ";", &saveptr);

        while (command != NULL) {
            execute_command(command);
            command = strtok_r(NULL, ";", &saveptr);
        }
    }

    return 0;
}

