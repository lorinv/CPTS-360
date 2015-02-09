/***************** pipe.c file ***********************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pd[2];
char buf[128];

main(int argc, char *argv[], char *env[])
{
   int r, n, pid;
   int i; int c;

   r = pipe(pd);  // system call to create a pipe' pd[0]=READ, pd[1]=WRITE
   printf("pd=[%d, %d]\n", pd[0], pd[1]);
 
   pid = fork();    // fork a child process, which has the SAME pd[ ]

   if (pid){  // parent as pipe WRITER
      printf("Child %d write to pipe\n", getpid());
      close(pd[1]);// WRITER MUST close pd[0]
      close(0);    // close 1
      dup(pd[0]);  // replace 1 with pd[1]

      printf("CHILD WRITES LINES TO PIPE\n");

      for (i=0; i < 10; i++){
          fprintf(stderr, "PARENT WRITES LINE %d TO STDOUT\n", i);
	  printf("this is line %d from parent\n", i);
      }
   }
   else{            // child as pipe READER
      printf("parent %d  read from pipe\n", getpid());
      close(pd[0]); // READER MUST close pd[1]
      close(1);  
      dup(pd[1]);   // replace 0 with pd[0]

      while( (c = getc(stdin)) != EOF){
	putchar(c);
      }

      printf("---------------------------------------\n");
      printf("child: no more\n");
  }
}

/*

                         QUESTIONS:

1. What's a pipe? _____________________________________________________
	- chains processes so that the standard out of one flows directly 
		into the standard in of another. 

2. The parent is the pipe WRITER. How does it replace its fd=1 with pd[1]?
	- Closes pd[0], then uses the dup command. These system calls
	 create a copy of the file descriptor oldfd.

       dup()  uses  the  lowest-numbered  unused  descriptor 
		 for the new descriptor.


3. The child is the pipe READER. How does it replace its fd=0 with pd[0]?
	- Closes pd[1], then uses the dup command.
	- Same as above

4. MODIFY the code to let the parent be the READER and the child the WRITER.
   Test run the program again.
    
*/
