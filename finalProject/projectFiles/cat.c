#include "util.h"
#include <stdio.h>

int cat_file()
{
	char buf[BLKSIZE], dummy = 0;
	int n, fd;

	//fd = open filename for READ
	while (n = read(fd, buf[1024], 1024))
	{
		buf[n] = 0;
		//TODO: Fix this if you have time
		printf("%s", buf);
	}
	close(fd);
}	 
