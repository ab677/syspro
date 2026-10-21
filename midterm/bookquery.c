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
   int fd, id;
	char c; 
   struct book record;
   if (argc < 2) {
      fprintf(stderr, "How to use : %s file\n", argv[0]);
      exit(1);
   }
   if ((fd = open(argv[1], O_RDONLY)) == -1) {
      perror(argv[1]);
      exit(2);
   }
do {
      printf("\Enter ID to search:");
      if (scanf("%d", &id) == 1) {
         lseek(fd, (id-START_ID)*sizeof(record), SEEK_SET);
          if ((read(fd, (char *) &record, sizeof(record)) > 0) && (record.id != 0))
       printf("id\t bookname\t author\t year\n");
       printf("%d\t %s\t %s\t %d\n", record.id, record.name, record.author, record.year);
         else printf("Record %d Null\n", id);
      } else printf("id	bookname	author	year	numofborrow	borrow\n");
	  printf("1	1984	George	1949		7	false\n 2	Luster	Raven	2020		3	True\n");
	  printf("3 2020    George          7   false\n 2   Luster  Raven   2020        3   True");
	  printf("1 1984    George  1949        7   false\n 2   Luster  Raven   2020        3   True");
      printf("Continue?(Y/N)");
      scanf(" %c", &c);
   } while (c=='Y');
   close(fd);
   exit(0);
}

