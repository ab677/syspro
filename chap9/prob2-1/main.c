#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
pid_t wait(int*status);
pid_t waitpid(pid_t, int * statloc, int options);

int main() 
{
   int pid, child, status;
   printf("[%d] parental process start \n", getpid( ));
   pid = fork();
   if (pid == 0) {
      printf("[%d] child process start \n", getpid( ));
      exit(1);
   }
   child = wait(&status);
   printf("[%d] child process %d end \n", getpid(), child);
   printf("\tend code %d\n", status>>8);
}

