/*************** a.c file **************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pid;

char line[128];
char token[32][64];
char cwd[128], command[128];

main(int argc, char *argv[ ])
{

/*
  1. DRAW A DIAGRAM TO SHOW EXACTLY WHAT IS argv:

     // DEFINE myargv as AN ARRAY OF 32 char pointers:
     _____????_____ myargv___???____;
		

  2. Write code to           |-------- n=5--------| 
        input a line, e.g.    b.out this is a test
        tokenize line into inidividual strings as 
           token[0], token[1],....,token[n-1]
        
  3. // Write code to let myargv[i] point to token[i], (0 <= i < n)
        myargv[n] = 0;
*/

  char *myargv[32];
  char input[256];
  printf("Type Things Please: ");
  fgets(input, 255, stdin);
  printf("input: %s\n", input);
  int i = 1;
 
  char * pch = strtok (input, " ");
  while (pch != NULL)
  {
	myargv[i] = pch;
    pch = strtok (NULL, " ");
	i++;
  }
  myargv[i] = 0;
  
  pid = getpid();
  printf("proc %d in a.out exec to b.out\n", pid);

  getcwd(cwd,128);     // cwd contains the pathname of CWD  
  printf("cwd = %s\n", cwd);

  // WRITE CODE to let command = CWD/b.out
  // command = CWD/b.out;
  strcpy(command, strcat(cwd, "/b.out")); 

  execve(command, &myargv, NULL);

  printf("execve failed\n");
}

/*

                   QUESTIONS:
Which process executes a.out? 3386 
which process executes b.out? 3386
 
what are the argv[] strings in b.out? 
	- name of file and myargv (The tokenized string
	  from user input)

     HOW TO PASS env[ ] strings 
                 PATH=/a/b/c
                 HOEM=/root
                 TERM=console
     to main(int argc, char *argv[ ], char *env[ ]);

*/

