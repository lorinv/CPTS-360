#include "util.h"
#include <stdio.h>

int cat_file()
{
	char buf[BLKSIZE], dummy = 0;
	int n, fd;

	printf("pathname %s\n", pathname);
	fd = open(pathname, O_RDONLY);
	if (fd == -1)
	{
		printf("Invalid file descriptor.\n");
		return -1;
	}
	printf("FD %d\n", fd);	

	//fd = open filename for READ
	while (n = read(fd, buf, 1023))
	{
		buf[n] = 0;
		//TODO: Fix this if you have time
		printf("%s", buf);
	}
	close(fd);
}	 
