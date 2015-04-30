#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

#include "util.h"

extern PROC* running;
extern MINODE *root;

//Copies a block from a given location and file
//descriptor into a given buffer
int get_block(int fd, int blk, char *buf)
{
	lseek(fd, (long)(blk * BLOCK_SIZE), 0);
	return read(fd, buf, BLOCK_SIZE);
	//Return -1 = error
	//Return  0 = (EOF)
}

//Puts the block stored in the buffer back into the 
//device at the given location
int put_block(int dev, int blk, char *buf)
{
	lseek(dev, (long)(blk * BLOCK_SIZE), 0);
	return write(dev, buf, BLOCK_SIZE);
}

int token_path(char *pathname, char **token_ptrs)
{
	int tok_i;
	token_ptrs[0] = strtok(pathname, "/\n");
	
	for (tok_i = 0; token_ptrs[tok_i]; tok_i++)
	{
		token_ptrs[tok_i + 1] = strtok(NULL, "/\n");
	}

	//Returns the number of tokens
	return tok_i;
}

char* dir_name(char* pathname)
{
	dirname(pathname);		
}

char* base_name(char* pathname)
{
	basename(pathname);
}

int getino (int* dev, char* pathname)
{
	int i, mip, ino;
	
	if (strcmp("/", pathname))
	{
		//Return root's ino
	}	
	else if (pathname[0] == '/')
	{
		//Start from root's ino and progress	
	}
	else
	{
		//get the ino from the running process's 
		//current working directory
		ino = running->cwd->ino;
		*dev = running->cwd->dev;
	}
	
	char pathNameTokenized[128];
	char *pathNameTokenPtrs[120];
	int tokenCount;

	strncpy(pathNameTokenized, pathname, strlen(pathname) + 1);
	tokenCount = token_path(pathNameTokenized, pathNameTokenPtrs);
	
	if (tokenCount == 0) return -1;

	for (i = 0; i < tokenCount; i++)
	{
		mip = iget(*dev, ino);
		ino = search(mip, pathNameTokenPtrs[i]);	
	}
	return ino;
}

int search (MINODE *inodePtr, char *name)
{
	char buf[1024];
	DIR *dp = (DIR *)buf;
	char *cp = buf;
	int ino;
	int x = 0, i = 0;
	char c;

	for (; i < 12; i++)
	{
		get_block(inodePtr->dev, inodePtr->INODE.i_block[i], buf);

		while (cp < buf + BLKSIZE)
		{
			if (strncmp(name, dp->name, strlen(name)) == 0 && dp->name_len == strlen(name))
			{
				ino = dp->inode;
				return ino;
			}

			cp += dp->rec_len;	//move cp to start of the next Inode
			dp = (INODE *)cp;   //Gets the inode at cp
		}
	}
	//If name wasn't found
	return 0;
}
		
/*
Check to see if INODE is already in the MINODE array
If it is return it
Else get the block number that the INODE will be in on the disk
by dividing by 8. Mod by 8 to get the offset. Setup a pointer 
casted to an INODE that points at the block.

Once you have the inode, find the next cell in the MINODE array
and then copy the contents on the INODE into that cell
return a reference to that cell.
*/
MINODE* iget (int dev, u32 ino)
{
	int i, blk, offset;
	char buf[BLKSIZE];

	//Check to see if the INODE we want is already in memory
	//if so, just point to that.
	for (i = 0; i < NMINODES; i++)
	{
		if (minode[i].dev == dev && minode[i].ino == ino && minode[i].refCount > 0)
		{
			minode[i].refCount++;
			return &minode[i];
		}
	}

	//Gets the respective block the INODE is located
	//Accounting for the appropriate offset
	blk = (ino - 1)/8 + INODEBLOCK; 
	offset = (ino - 1) % 8;
	get_block(dev, blk, buf);
	//Points at the respective inode
	ip = (INODE*)buf + offset;
	
	//Now that we got the INODE from the disk
	//we need to store it in memory as a MINODE
	for (i = 0; i < NMINODES; i++)
	{
		//Go through the array of MINODES
		//Find the first one that is free
		if (minode[i].refCount == 0)
		{
			minode[i].INODE = *ip;
			minode[i].dev = dev;
			minode[i].ino = ino;
			minode[i].refCount = 1;
			minode[i].dirty = 0;
			minode[i].mounted = 0;
			minode[i].mountptr = 0;
			return &minode[i];	
		}
	}
	
	//IF the array is somehow full
	return NULL; 
}

//We need to write the INODE back to the disk
//So the changes are applied
/*
	Decrease the reference count
	Check if the INODE is still being referenced
	if not, get the disk block using the ino and
	the appropiate offset
	Copy the info from the MINODE to the INODE on the
	disk.
	Use the put block function to apply to the block
	to the disk
*/
int iput (MINODE *mip)
{
	int blk, offset;
	char buf[BLKSIZE];

	mip->refCount--;
	if (0 < mip->refCount) return; 
	if (!mip->dirty) return;	
	//If nothing is currently referncing the 
	//inode and it's been changed
	//put it back
	if (0 == mip->refCount && mip->dirty)
	{
		//use -1 because inodes index from 1
		blk = (mip->ino - 1)/8 + INODEBLOCK;
		offset = (mip->ino - 1) % 8;
		get_block(mip->dev, blk, buf);
		INODE* ip2 = (INODE*)buf + offset;

		//You have to actually copy the space
		//Not just point to it
		memcpy(ip2, &mip->INODE, sizeof(INODE));
		put_block(mip->dev, blk, buf);
	}
}

int findmyname(MINODE *parent, int myino, char *myname)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
}

int findino(MINODE *mip, int *myino, int *parentino)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
}

int ialloc(dev)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
}

int balloc(dev)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
}

//Deallocate an ino
int idealloc(int dev, int ino)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
}

//deallocate a bno
int bdealloc(int dev, int bno)
{
	printf("NOT DONE YET!!!!!!!!!!!!!!!!!!!!!!!");
	return 0;
} 
