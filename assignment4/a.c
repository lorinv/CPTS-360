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

  #define char *myargv[32];
  int i = 0;
  for (; i < argc; i++)
  {
	myargv[i] = argv[i];
  }
  myargv[i] = 0;
  
  pid = getpid();
  printf("proc %d in a.out exec to b.out\n", pid);

  getcwd(cwd,128);     // cwd contains the pathname of CWD  
  printf("cwd = %s\n", cwd);

  // WRITE CODE to let command = CWD/b.out
  //command = CWD/b.out;

  execve(command, &myargv);

  printf("execve failed\n");
}

/*

                   QUESTIONS:
Which process executes a.out?_________________________________________
which process executes b.out?_________________________________________
 
what are the argv[] strings in b.out? ______________________________

     HOW TO PASS env[ ] strings 
                 PATH=/a/b/c
                 HOEM=/root
                 TERM=console
     to main(int argc, char *argv[ ], char *env[ ]);

*/

