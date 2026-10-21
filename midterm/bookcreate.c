#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 24
#define START_ID 1001001
struct book {
   char name[20];
   int id;
  char author[15];
   short year;
};
int main(int argc, char *argv[])
{
   int fd;
   struct book record;
   if (argc < 2) {
      fprintf(stderr, "How to use: %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0640)) == -1) {
      perror(argv[1]);
      exit(2);
   }
   printf("%-9s %-8s %-7s %-4s\n", "ID", "Name", "author" "Score");
   while (scanf("%d %s %d", &record.id, record.name, record.author, &record.year) == 4) {
      lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
      write(fd, (char *) &record, sizeof(record) );
   }
   close(fd);
   exit(0);
}

