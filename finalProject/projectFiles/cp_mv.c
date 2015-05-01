//#include "util.h"

int cp_file()
{
	int n, ino, dev;
	int fd, gd;
	char buf[BLKSIZE];
	MINODE *mip1, *mip2;

	if (strcmp(pathname, "") == 0 || strcmp(parameter, "") == 0)
	{
		printf("Please supply both the src and destination.\n");
		return 0;
	}
	
	//open src fpr read
	//O_RDONLY = 0
	fd = running->fd[open_file(pathname, 0)];
	dev = running->cwd->ino;
	ino = getino(dev, pathname);
	mip1 = iget(dev, ino);	

	fd = running->fd[open_file(parameter, 0)];
	ino = getino(dev, parameter);
	mip2 = iget(dev, ino);
	
	int *tempptr;
		
	if (fd == -1)
	{
		printf("Invalid File descriptor...");
		return 0;
	}

	//open dst for WR
	//gd = open_file("tiny2", O_RDWR);	

	while(n = read(fd, buf, BLKSIZE))
	{
		write(gd, buf, n);
	}
}

void mv_file()
{
	//verify src exists
	char * src = pathname;
	char * dest = parameter;
	int fd, ino;
	int dev = running->cwd->dev;
	MINODE *mip1, *mip2;

	ino = getino(dev, src);
	mip1 = iget(dev, ino);
	
	ino = getino(dev, dest);
	mip2 = iget(dev, ino);

	if (mip1->dev == mip2->dev)
	{
		link(src, dest);
		unlink(src);	
	}	
	else
	{
		cp_file();
		unlink(src);
	}	
}
