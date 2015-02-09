/******************** t2.c *********************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *p = 0;
main()
{
	int pid, status;
	int exitValue;

	printf("PROC %d forks a child\n", getpid());

	pid = fork();

	if (pid)
	{ // PARENT:
		printf("\nPARENT %d WAITS FOR CHILD %d TO DIE\n", getpid(),pid);
       pid=wait(&status);
       printf("Parent %d: DEAD CHILD=%d, status=%04x\n", getpid(), pid, status);
	}
	else
	{// child:
		printf("this is child proc %d, input a value (0-255) to die: ", getpid());
		scanf("%d", &exitValue);     
		printf("child %d dies by exit(%d)\n", getpid(), exitValue);
HERE:	{ int a,b; a = 1; b = 0; a = a/ b; }
		exit(exitValue); 
	}
}

/*
                         QUESTIONS:
(1). What's the value of status in pid=wait(&status)?
	- 0100	

	What's the relation between the exitValue in exit(exitValue) and status in wait(&status)?
	- Status can be used with listed macros to determine the exit 
	value of the child or even if the child was terminated.
	- " If  status is not NULL, wait() and waitpid() store status
	 information in the int to which it points.  This integer
	 can be inspected with  the  following  macros  (which
	 take the integer itself as an argument, not a pointer to it, as is		done  in  wait()  and  wait‐ pid()!): "



(2). Insert *p = 1234; to HERE:
     Run the program again, and answer (1) again: 008b
	

(3). Insert { int a,b; a = 1; b = 0; a = a/ b; } to HERE:
     Run the program again, and answer (1) again:? 0088
	 

*/
