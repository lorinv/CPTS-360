/************* io.c file ********************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

main()
{
     int fd;
L1:  printf("this is PROC %d printing to stdout\n", getpid());

     fd = open("myfile", O_WRONLY|O_CREAT, 0644);
     printf("fd = %d\n", fd);

     close(1);
     dup(fd);

     getchar();

L2:  printf("where does this line go?\n");
L3:  printf("and this line?\n");
}

/*
    -------------------- QUESTION: ------------------------------
At L1:     the printed line will show up on the screen.
At L2, L3: where do the printed lines go?

 Not Printed to screen because the "close" command deallicated the file descriptor for standard out.  Any other output is discared.

*/
