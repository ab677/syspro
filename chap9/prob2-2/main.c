#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *statloc, int options);

int main()
{
    int pid1, pid2, child, status;

    printf("[%d] Parental process start\n", getpid());

    pid1 = fork();

    if (pid1 == 0) {
        printf("[%d] Child process [1] start\n", getpid());
        sleep(1);
        printf("[%d] Child process [1] end\n", getpid());
        exit(1);
    }

    pid2 = fork();

    if (pid2 == 0) {
        printf("[%d] Child process [2] start\n", getpid());
        sleep(2);
        printf("[%d] Child process [2] end\n", getpid());
        exit(2);
    }

    child = waitpid(pid1, &status, 0);

    printf("[%d] Child process [1] %d end\n",
           getpid(), child);

    printf("\tend code: %d\n", status >> 8);
}
