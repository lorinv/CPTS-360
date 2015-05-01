//#include "util.h"
#include <stdio.h>

extern int extFD;

int write_file()
{
	//Temp variables
	//TODO
	int fd = extFD;
	char newStr[256];

	//Check if open and if eith WR or RW or APPEND
	if ((running->fd[fd] && running->fd[fd]->refCount) &&
		(running->fd[fd]->mode == 1 || running->fd[fd]->mode == 2 ||
			running->fd[fd]->mode == 3))
	{
		//copy text string into a buf[] and get its length as nbytes
		//TODO
		printf("Enter string to write to file: ");
		fgets(newStr, 256, stdin);
		return(mywrite(fd, newStr, strlen(newStr)));	
	}
	else
	{
		printf("File not open in the corrent format.\n");
		return -1;
	}
}

int balloc (int dev)
{
    int i;
    char buf[BLOCK_SIZE];
    int nblocks; 

    get_block(dev, SUPERBLOCK, buf);
    sp = (SUPER*)buf;
    nblocks = sp->s_blocks_count;

    get_block(dev, BBITMAP, buf);
    for (i = 0; i < nblocks; i++)
    {
        if (buf[i/8] & (1 << (i%8)) == 0)
        {
			buf[i/8] |= (1 << (i%8));
            put_block(dev, BBITMAP, buf);

            decFreeBlocks(dev);
            return (i + 1);
        }
    }
    return 0;
}

int mywrite(int fd, char buf[], int nbytes)
{
	//int i, j;
	int lbk, startByte, blk, offset;
	OFT* oftp;
	char wbuf[BLKSIZE];
	int remain;
	MINODE *mip;
	char *cq = buf;
	//int ibuf[BLKSIZE / 4]; //For indirect blocks
	//int dibuf[BLKSIZE / 4]; //For direct blocks

	//Get the INODE of the current file you're working with
	//Open file table
	oftp = running->fd[fd];
	mip = oftp->inodeptr;
	printf("Writing to ino = %d\n", mip->ino);
	int numBytes = nbytes;
	while (nbytes > 0)
	{
		//Compute logical block (lbk) and the startByte
		//Because we don't always want to start at the 
		//beginning of the file
		lbk = oftp->offset / BLKSIZE;
		startByte = oftp->offset & BLKSIZE;
		printf("lbk = %d\n", lbk);
		if (lbk < 12)
		{
			if (mip->INODE.i_block[lbk] == 0)
			{
				//Must allocate a block
				mip->INODE.i_block[lbk] = balloc(mip->dev);
				
				//Write a block of 0's to blk on disk
				// 
			}
			blk = mip->INODE.i_block[lbk];
			printf("mip=>dev: %d\n", mip->dev);
		}
		else if (lbk >= 12 && lbk < 256 + 12)
		{
			/*
			//Indirect blocks
			get_block(mip->dev, mip->INODE.i_block[12], (char *) ibuf);
			
			for(i = 0; i < 256; i++)
			{
				if(ibuf[i] == 0)
				{
					ibuf[i] = balloc(mip->dev);
					blk = ibuf[i];
					put_block(mip->dev, mip->INODE.i_block[12], (char *) ibuf);
					break;
				}
			}
			*/
		}
		else
		{
			//Double indirect blocks
			/*
			blk = -1;
			get_block(mip->dev, mip->INODE.i_block[13], (char *) ibuf);
			
			for (i = 0; i < 256; i++)
			{
				if (ibuf[i])
				{
					get_block(mip->dev, ibuf, (char *)dibuf);
					for (j = 0; j < 256; j++)
					{
						if (dibuf[j] == 0)
						{
							dibuf[j] = balloc(mip->dev);
							blk = dibuf[j];
							put_block(mip->dev, ibuf[i], (char *)dibuf);
							break;
						}
					}
				}
			}
			*/
		}

		get_block(mip->dev, blk, wbuf);		
		char *cp = wbuf + startByte;
		remain = BLKSIZE - startByte;
		
		while (remain > 0)
		{
			*cp++ = *cq++;
			nbytes--; remain--;
			oftp->offset++;
			if (offset > mip->INODE.i_size)
				mip->INODE.i_size++;
			if (nbytes <= 0) break;
		}
		put_block(mip->dev, blk, wbuf);
	
		//Loop back to write more until all are writen
	}
	
	mip->dirty = 1; //mark mip dirty for iput()
	printf("Write %d char into file descriptor fd=%d\n", numBytes, fd);
	return nbytes;
}
