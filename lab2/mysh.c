/*
Lorin Vandegrift
11354621
02/17/2015
Write C program which simulates the Unix sh for command processing.
*/

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	//Variables
	char *myargv[32];
	char input[256];
	printf("Input: ");
	fgets(input, 255, stdin);
	printf("Input: %s\n", input);
	int i = 0;

	char * pch = strtok (input, " ");
	while (pch != NULL)
	{
		myargv[i] = pch;
		pch = strtok (NULL, " ");
		i++;
	}
	myargv[i] = 0;	
	
	//cmd = "cd" : chdir(arg1) OR chdir($HOME) if no arg1
	
	//cmd = "exit" : exit(1) to terminate;
	
	/*
	For all other commands:
		for a child process
		wait for the child to terminate
		print childs exit status code
		continue to step 1
	*/

	/*
	Handle I/O redirection
	*/

	/*
		Execute cmd by execve(), passing parameters
	*/	
}

