#include "util.h"
#include <stdio.h>

int lseek_file (int fd, int position)
{
	OFT* oftp;
	
	if (running->fd[fd] && running->fd[fd]->refCount > 0)
	{
		oftp = running->fd[fd];
		
		if (position <= (oftp->inodeptr->INODE).i_size)
		{
			oftp->offset = position;	
			running->fd[fd] = oftp;
		}	
		else
		{
			printf("Position out of bound...\n");
			return -1;
		}
		return position;
	}
	else
	{
		return -1;
	}
}
