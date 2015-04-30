
void cp_file(int fd, in gd)
{
	int n;	

	//open src fpr read

	//open dst for WR

	while(n = read(fd, buf[], BLKSIZE))
	{
		write(gd, buf, n);
	}
}

void mv_file(int src, int dest)
{
	//verify src exists
		
}
