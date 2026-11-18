#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{ 
   int child, pid, status;
   pid = fork( );
   if (pid == 0) {
      execvp(argv[1], &argv[1]);
      fprintf(stderr, "%s:err\n",argv[1]);
   } else {
      child = wait(&status);
      printf("[%d] child process %d end \n", getpid(), pid);
      printf("\tend cod %d \n", status>>8);
   }
}

