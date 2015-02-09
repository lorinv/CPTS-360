/***************** b.c file ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char *argv[ ])
{
  int i = 0;
  printf("proc %d in b.out: argc=%d\n", getpid(), argc);

  // WRITE YOUR CODE TO PRINT arv strings
  // You can start i at 0 if you want to print the file
  // name
  for(i = 1; i < argc; i++)
  {
	printf("%s\n", argv[i]);
  }

  printf("proc %d in b.out exit\n", getpid());
}
/************** end of b.c file ******************/


