//Lorin Vandegrift
//11354621

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc	GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode		INODE;
typedef struct ext2_dir_entry_2 DIR;	// need this for new version of e2fs

GD	  *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 
int fd;

#define BLKSIZE 1024

//Given a file system, a block number, and buffer
//This function with save the block in the buffer
void get_block(int fd, int blk, char buf[BLKSIZE])
{
	lseek(fd, (long)(blk*BLKSIZE), 0);
	read(fd, buf, BLKSIZE);
}

// Given a path and an Inode, it will find the corresonding data blocks
int search (INODE *inodePtr, char *name)
{
	//Search for each part of your pathname
	char buf[1024];
	
	//DP  = Directory pointer
	DIR  *dp = (DIR *)buf;	  // access buf[] as DIR entries
	//CP - Charactor pointer, used to keep track if you've
	//Looked through everything in the buf
	char *cp = buf;			   // char pointer pointing at buf[ ]
	//Inumber - Represents Files
	int ino;
	int x = 0;
	char c;
	int i = 0;

   //Go throught the 12 Direct INode blocks
   for (; i < 12; i++)
   {
	   get_block(fd,inodePtr->i_block[i], buf);
		//read a block into buf[ ];
		
		//If the inode is not empty
		//If you find a Inode that is 0, you can stop looking
		//Because the rest of the Inodes will also be null, empty
		if (dp->inode != 0)
		{
			//Go through the inode and compare the given name to the name in the Inode
			//Until you find a match
			while(cp < buf + BLKSIZE)
			{
			   printf("name: %.*s\n", dp->name_len, dp->name);
				
			   if (strncmp(name, dp->name, strlen(name)) == 0 && dp->name_len == strlen(name))
			   {
				   ino = dp->inode;
				   printf("ino: %d\n", ino);
				   return ino;
			   }
				
			   cp += dp->rec_len;		  // advance cp by rlen in bytes
			   dp = (INODE *)cp;	   // pull dp to the next DIR entry
			}
		}
	}

	return 0;
}

int main (int argc, char *argv[])
{
	char buf[1024];
	SUPER * ext = buf;

	//Opens the disk for Read Only
	fd = open(argv[1], O_RDONLY);

	//Gets the second block, the super block,
	//To check if it's a EXT2 filesystem
	get_block(fd, 1, buf);
	if (ext->s_magic == 61267) 
	{
		get_block(fd, 2, buf);
		GD * gd = buf;
		int InodesBeginBlock = gd[0].bg_inode_table; 
		printf("Inodes begin: %d\n", InodesBeginBlock);		
	
		//Gets the inode of root /
		get_block(fd, InodesBeginBlock, buf);
		INODE * ip = (INODE *)buf + 1; //Gets root	
		
		//Break up pathname using strtok
		char * name[10];
		char * pch;
		int z = 0;
		pch = strtok (argv[2],"/");
		while (pch != NULL)
		{
			//printf ("%s\n",pch);
			name[z] = pch;
			pch = strtok (NULL, "/");
			z++;
		}		
	
		int inumber = 0;
		int j = 0;
		int index, block;
		for (; j < z; j++)
		{
			inumber = search(ip, name[j]);
			if (inumber == 0)
			{
				printf("Inode not found...\n");
				exit(1);
			}
			//Read in correct block
			block = ((inumber-1) / 8) + InodesBeginBlock;
	        index = (inumber-1) % 8;
            printf("inumber: %d\n", inumber);
 
            get_block(fd, block, buf);
            ip = (INODE *)buf + index;

		}

		block = ((inumber-1) / 8) + InodesBeginBlock;
		index = (inumber-1) % 8;
		printf("inumber: %d\n", inumber);

		get_block(fd, block, buf);
		ip = (INODE *)buf + index;
	
		printf("Disk Blocks: \n");
		for (j = 0; j < 15; j++)
		{	
			printf("%d\n", ip->i_block[j]);
		}
		

		printf("\n\nDirect Blocks: \n");
		for (j = 0; j < 12; j++)
		{
			if(ip->i_block[j] != 0)
			{
				printf("%d\n", ip->i_block[j]);
			}
		}

		printf("\n\nIndirect Blocks: \n");
		get_block(fd, ip->i_block[12], buf);
		int * tempPtr = buf;
		j = 0;
		while (tempPtr != 0 && tempPtr < buf + BLKSIZE)
		{
			if (*tempPtr != 0)
			{
				printf(" %d", *tempPtr);
				if (j % 10 == 0) printf("\n");
				j++;	
			}
			tempPtr++;
		}

		char buf2[1024];
		printf("\n\nDouble Indirect Blocks: \n");
		get_block(fd, ip->i_block[13], buf);
		get_block(fd, buf[0], buf2);

		for (j = 0; j < 256; j++)
		{
			if(buf2[j] != 0)
			{
				printf("%d ", buf2[j]);
				if (j % 10 == 0) printf("\n");
			}
		}		
/*
		tempPtr = buf;
		char buf2[1024];
		int * tempPtr2 = buf2;
		j = 0;
		while (tempPtr != 0 && tempPtr < buf + BLKSIZE)
		{
			//get_block(fd, *tempPtr, buf2);
			while (tempPtr2 != 0 && tempPtr2 < buf2 + BLKSIZE)
			{
				if (*tempPtr2 != 0)
				{
					printf(" %d", *tempPtr2);
					if (j % 10 == 0) printf("\n");
					j++;	
				}
				tempPtr2++;
			}
			tempPtr++;
		}
*/
		

	}
	else
	{
		printf("Your filesystem is not a EXT2 filesysystem.\n");
	}	
}


