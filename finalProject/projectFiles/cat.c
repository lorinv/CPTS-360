//#include "util.h"
//#include <stdio.h>

int cat_file()
{
	char buf[BLKSIZE], dummy = 0;
	int n, fd;

	printf("pathname %s\n", pathname);
	//fd = open(pathname, O_RDONLY);
	fd = open_file2(pathname, 0);
	if (fd == -1)
	{
		printf("Invalid file descriptor.\n");
		return -1;
	}
	printf("FD %d\n", fd);	

	//fd = open filename for READ
	n = myread(fd, buf, 1020);
	while (n != 0)
	{
		buf[n] = 0;
		printf("%s", buf);
		n = myread(fd, buf, 1020);
	}
	close_file2(fd);

}	 
