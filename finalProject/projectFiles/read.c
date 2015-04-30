

int read_file()
{

	//temp fd and nbytes
	int fd = 0;
	int nbytes = 0;
	
	char buf[nbytes + 1];

	if ((!running->fd[fd] && running->fd[fd]->refCount == 0) ||
		(running->fd[fd]->mode != 0 && running->fd[fd]->mode != 2))
	{
		prinf("Not opened for RD or RW.\n");	
		return -1;
	}
	else
	{
		//Do I need dynamic memory here?
		return (myread(fd, buf, nbytes));
	}

}

int myread(int fd, char *buf, int nbytes)
{
	int count = 0;
	char *cq, buf[BLKSIZE], buf2[BLKSIZE];
	oftp = running->fd[fd];
	int lbk, blk, startByte, remain;
	char readbud[BLKSIZE];

	int avil = mip->INODE.i_size - oftp->offset;
	cq = buf;

	while (nbytes && avil)
	{
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
			get_block(mip->dev, mip->INODE.i_block[lbk], buf);
			//You can them refernce them directly, but they start
			//at 12
			blk = buf[lbk - 12]; 
		}
		else
		{
			//double indirect blocks
			get_block(mip->dev, mip->INODE.i_block[13], buf);
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
