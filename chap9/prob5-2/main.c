#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int child, pid, fd, status;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s filename command [args...]\n", argv[0]);
        return 1;
    }
    pid = fork();
    if (pid == 0) {
        fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
        if (fd == -1) {
            perror("open");
            _exit(1);
        }
        dup2(fd, 1);
        close(fd);
        execvp(argv[2], &argv[2]);
        fprintf(stderr, "%s: err\n", argv[2]);
        _exit(127);
    } else if (pid > 0) {
        child = wait(&status);
        printf("[%d] child process %d end\n",
               getpid(), child);
    } else {
        perror("fork");
        return 1;
    }
    return 0;
}

