/*
	Answers to the questions are at the bottom of each file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main()
{
	int pid;
	printf("THIS IS %d  MY PARENT=%d\n", getpid(), getppid()); 

	pid = fork();   // fork syscall; parent returns child pid, 
					// child returns 0
	if (pid < 0)
	{				// fork() may fail. e.g. no more PROC in Kernel
		perror("fork faild");
		exit(1);
	}
  
	if (pid)
	{				// PARENT EXECUTES THIS PART 
		printf("THIS IS %d  MY PARENT=%d\n", getpid(), getppid() ); 
	}
	 
	else
	{ // child executes this part
		printf("I am %d my parent=%d\n", getpid(), getppid());	
	}
}

/*
                        QUESTIONs: 
What are the pid and ppid of the process that executes a.out?5012, 3571
What are the pid and ppid of the CHILD process? 5013, 5012

Run a.out several times: which pid does NOT change and WHY? 
	The parent of the 5012, PID 3571. It does not change because the parent process is still running. Until the process dies, it will keep the same PID.

Output:
THIS IS 5012  MY PARENT=3571
THIS IS 5012  MY PARENT=3571
THIS IS 5012  MY PARENT=3571
I am 5013 my parent=5012

*/
