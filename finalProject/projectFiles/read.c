

int read_file()
{
	int fd, nbytes;
	
	if (strcmp(pathname,"") == 0 || strcmp(parameter,"") == 0)
	{
		printf("Please enter both a file descriptor and the number of bytes.\n");
		return -1;
	}

	fd = atoi(pathname);
	nbytes = atoi(parameter);
	
	char buf[nbytes + 1];

	//check is file is open, is being referenced, and 
	//Is either in read or readwrite mode
	if ((!running->fd[fd] && running->fd[fd]->refCount == 0) ||
		(running->fd[fd]->mode != 0 && running->fd[fd]->mode != 2))
	{
		printf("Not opened for RD or RW.\n");	
		return -1;
	}
	else
	{
		return (myread(fd, buf, nbytes));
	}

}

//reads from a file descriptor
//Reads up to nbytes from fd into buf
int myread(int fd, char *buf, int nbytes)
{
	int count = 0;
	char *cq;
	int buf2[BLKSIZE];
	OFT* oftp; 
	int lbk, blk, startByte, remain;
	int ino;
	char readbuf[BLKSIZE];
	MINODE *mip;

	//Need to get the MINODE
	oftp = running->fd[fd];
	mip = oftp->inodeptr;

	//Number of bytes still available in the file
	int avil = mip->INODE.i_size - oftp->offset;
	cq = buf;

	//This loop basically goes through the file to be read
	//and copies each block into the provided buffer
	while (nbytes && avil)
	{
		//Compute logical block # (lbk) and startByte
		//startByte in that block from offset
		lbk = oftp->offset / BLKSIZE;
		startByte = oftp->offset % BLKSIZE;

		if (lbk < 12)
		{
			blk = mip->INODE.i_block[lbk];
		}
		else if(lbk >= 12 && lbk < 256 + 12)
		{
			//indirect blocks
			//Gets the 255 cells and copies them into buf
			get_block(mip->dev, mip->INODE.i_block[lbk], buf2);
			//You can them refernce them directly, but they start
			//at 12
			blk = buf[lbk - 12]; 
		}
		else
		{
			//double indirect blocks
			get_block(mip->dev, mip->INODE.i_block[13], buf2);
			get_block(mip->dev, buf[((lbk - 12) / 256) - 1], buf2);
			blk = buf2[(lbk - 12) - (256 * ((lbk - 12) / 256))];
		}
		
		//Put data block into readbuf
		get_block(mip->dev, blk, readbuf);
		
		//copy from startByte to buf[]
		//at most remain bytes in this block
		char *cp = readbuf + startByte;
		remain = BLKSIZE - startByte;
		while (remain > 0)
		{
			*cq++ = *cp++;
			oftp->offset++;
			count++;
			avil--; nbytes--; remain--;
			if (nbytes <= 0 || avil <= 0)
				break;
		}
		
		// if one data block is not enough, 
		//loop back to OUTER while for more	
	}	
	printf("myread: read %d char from file descriptor %d\n", count, fd);
	return count; //count is the actual number of bytes read
}
