//This file holds all the remaining parts of the final project I need to do.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc	GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode		INODE;
typedef struct ext2_dir_entry_2 DIR;

GD	  *gp;
SUPER *sp;
INODE *ip;
DIR   *dp; 
int fd;

#define BLKSIZE 1024

//////////////////////
//Helper Functions and Global Varibles
/////////////////////

void get_block(int fd, int blk, char buf[BLKSIZE])
{
	lseek(fd, (long)(blk*BLKSIZE), 0); //File descriptor, offset, start
	read(fd, buf, BLKSIZE);
}

// Given a path and an Inode, it will find the corresonding data blocks
int search (INODE *inodePtr, char *name)
{
	char buf[1024];
	DIR  *dp = (DIR *)buf;	
	char *cp = buf;			   

	//Inumber - Represents Files
	int ino;
	int x = 0;
	char c;
	int i = 0;

   //Go throught the 12 Direct INode blocks
   for (; i < 12; i++)
   {
	   get_block(fd,inodePtr->i_block[i], buf);
		
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
				   // inode container in dp is just a container for the inumber
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



/*

*/
void myOpen()
{
	//1. ask for a pathname and mode to open. You may use mode = 0|1|2|3 for RD|WR|RW|APPEND
	printf("Enter pathname and mode to open. \n 0|1|2|3 for RD|WR|RW|APPEND");

	char line[128];
	fgets(line, 128, stdin);
	line[strlen(line)-1] = 0;
	//if (line[0] == 0) continue;

	char * pch;
	char *input[10];
	int z = 0;
	pch = strtok (line," /");
	while (pch != NULL)
	{
		input[z] = pch;
		pch = strtok (NULL, " /");
		z++;
	}		
  /*
  2. get pathname's inumber:
         if (pathname[0]=='/') dev = root->dev;          // root INODE's dev
         else                  dev = running->cwd->dev;  
         ino = getino(&dev, pathname);  // NOTE: dev may change with mounting
  */
  
  int ino = search()

/*

  3. get its Minode pointer
         mip = iget(dev,ino);  

  4. check mip->INODE.i_mode to verify it's a REGULAR file and permission OK.
      
     Check whether the file is ALREADY opened with INCOMPATIBLE mode:
           If it's already opened for WR, RW, APPEND : reject.
           (that is, only multiple RD are OK)

  5. allocate a FREE OpenFileTable (OFT) and fill in values:
 
         oftp->mode = mode;      // mode = 0|1|2|3 for RD|WR|RW|APPEND 
         oftp->refCount = 1;
         oftp->minodePtr = mip;  // point at the file's minode[]

  6. Depending on the open mode 0|1|2|3, set the OFT's offset accordingly:

      switch(mode){
         case 0 : oftp->offset = 0; 
                  break;
         case 1 : truncate(mip);        // WR: truncate file to 0 size
                  oftp->offset = 0;
                  break;
         case 2 : oftp->offset = 0;     // RW does NOT truncate file
                  break;
         case 3 : oftp->offset =  mip->INODE.i_size;  // APPEND mode
                  break;
         default: printf("invalid mode\n");
                  return(-1);
      }

   7. find the SMALLEST i in running PROC's fd[ ] such that fd[i] is NULL
      Let running->fd[i] point at the OFT entry

   8. update INODE's time field. 
      for WR|RW|APPEND mode : mark Minode[ ] dirty

   9. return i as the file descriptor
*/
}

/*

*/
void myClose()
{
/*
  1. verify fd is within range.

  2. verify running->fd[fd] is pointing at a OFT entry

  3. The following code segments should be fairly obvious:
     oftp = running->fd[fd];
     running->fd[fd] = 0;
     oftp->refCount--;
     if (oftp->refCount > 0) return 0;

     // last user of this OFT entry ==> dispose of the Minode[]
     mip = oftp->inodeptr;
     iput(mip);

     return 0; 
*/
}

/*

*/
void myLseek()
{
/*
  From fd, find the OFT entry. 

  change OFT entry's offset to position but make sure NOT to over run either end
  of the file.

  return originalPosition
*/
}

/*

*/
void myRead()
{

}

/*

*/
void myWrite()
{

}

/*

*/
void myCat()
{

}

/*

*/
void myCp()
{

}

/*

*/
void myMv()
{

}

/*

*/
void myReaddir()
{

}

/*

*/
void myMount()
{

}

/*

*/
void myUnmount()
{

}

int main()
{
	printf("You compiled!\n");
}
