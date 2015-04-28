//Lorin Vandegrift
//11354621
//Lab 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <fcntl.h>
#include <ext2fs/ext2_fs.h>   // NOTE: Ubuntu users MAY NEED "ext2_fs.h"
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc  GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;    // need this for new version of e2fs


// Open File Table
typedef struct oft{
  int   mode;
  int   refCount;
  struct minode *inodeptr;
  int   offset;
}OFT;

// PROC structure
typedef struct proc{
  struct proc *next;  // for link list of PROCs
  int   uid;
  int   pid, gid;
  int   status;
  struct minode *cwd;
  OFT   *fd[10];
}PROC;
      
// In-memory inodes structure
typedef struct minode{		
  INODE INODE;               // disk inode
  int   dev, ino;
  int   refCount;
  int   dirty;
  int   mounted;
  struct mount *mountptr;
}MINODE;

int ninodes; //Number of Inodes
int nblocks;	//Number of blocks
int bmap; //bitmap of the blocks - which blocks are allocated
int imap; //bitmap for the inode - which inodes are allocated
int inodes_start; //Physical block where the inodes start
PROC P0;
PROC P1; 
PROC *running;// = P0;
MINODE *root;
int dev, fd;

#define BLKSIZE 1024


//ino = inode number
/*
Since each inode represents a file, we need to be able to address each inode.

*/
int getino(int dev, char *pathname)
{
		//Search for each part of your pathname
	char buf[1024];
	DIR  *dp = (DIR *)buf;	  // access buf[] as DIR entries
	char *cp = buf;			   // char pointer pointing at buf[ ]
	int ino;
	int x = 0;
	char c;
	int i = 0;

   //Go throught the 12 Direct INode blocks
   for (; i < 12; i++)
   {
	   //read a block into buf[ ];
	   //i - block #
	   //fd = file desciptor
	   get_block(fd,i, buf);
		
		//dp = directory pointer (defined above)
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
			if lower_bound[2] < 0:
				lower_bound = 0
			if upper_bound[2] > 255:
				upper_bound = 255
				
			   cp += dp->rec_len;		  // advance cp by rlen in bytes
			   dp = (INODE *)cp;	   // pull dp to the next DIR entry
			}
		}
	}

	return 0;
}   

get_block(int fd, int blk, char buf[BLKSIZE])
{
	lseek(fd, (long)(blk*BLKSIZE), 0);
	read(fd, buf, BLKSIZE);
}

MINODE *iget(int dev, int ino)
{
/*
	  Once you have the ino of an inode, you may load the inode into a slot in the Minode[] array. To ensure uniqueness, you must search the Minode[] array to see whether the needed INODE already exists:
  If you find the needed INODE already in a Minode[] slot, just inc its  refCount by 1 and return the Minode[] pointer. If you do not find it in memory, you must allocate a FREE Minode[i], load the INODE from disk into that Minode[i].INODE, initialize the Minode[]'s other fields and return its address as a MINODE pointer,
*/
	char buf[BLKSIZE];
	get_block(dev, ino, buf);
}

void init () // Initialize FS data structure
{
	*running = P0;

	P0.uid = 0;
	P0.cwd = 0;
	int i = 0;
	for (; i < 10; i++)
	P0.fd[i] = 0;	
	*(P0.next) = P1;

	P1.uid = 1;	
	P1.cwd = 0;
	for (i = 0; i < 10; i++)
	P1.fd[i] = 0;
	*(P1.next) = P0;	
}

void mount_root (char *disk) //Mount root file system, establish / and CWDs
{
	//dev = fd = open device for RW
	char buf[BLKSIZE];
	SUPER * ext = buf;
	dev = fd = open(disk, O_RDWR); //Correct this later
	
	//read SUPER block to verify it's an EXT2 FS;
	get_block(fd, 1, buf);
	if (ext->s_magic == 61267)
	{
		//get nblocks, ninodes; SAVE them AS GLOBALs
		ninodes = ext->s_inodes_count;
		nblocks = ext->s_blocks_count;	
	
		//read GD to get block numbers of bmap,imap,
		//inodes_start; save as globals
		get_block(fd, 2, buf);
		GD * gd = buf;
		bmap = gd[0].bg_block_bitmap;
		imap = gd[0].bg_inode_bitmap;
		inodes_start = gd[0].bg_inode_table;

		root = iget(dev, 2);	// get root inode 
		//(OPTIONAL): initialize mount table[0] for the mounted root device.
		//Since we already saved ninode, nblocks, bmap, imap, inodes_start as 
		//globals, it is NOT necessary to repeat them in mount[0].
		
		//Let cwd of both P0 and P1 point at the root minode (refCount=3)
		P0.cwd = iget(dev, 2); 
		P1.cwd = iget(dev, 2);
	}	
}

/*
//Prints out names and details (ls -l)
ls(char *pathname)
{
   int ino = getino(dev, pathname); //get inodes?
   MINODE *mip = iget(dev,ino);

	//Check if is a file
	//Take inode and bitmask it
   if (!S_ISDIR(mip->INODE.imode)
	  //Basename = /bla/bbla/bla/Lorin - base name is lorin
	  //List info from file
	  list_file(mip, basename(pathname));
   else
	  //Lists info from directory
		//List dir looks everything in dir and calls 
		//list_file on everything
	  list_dir(mip);

   iput(mip); //Put MINODE back onto disk
}

//Lists out the information of a regular file
int list_file(mip, name)
{
	//ip points to an INODE
   INODE *ip = &mip->INODE;
							 R L D
   //use ip->i_mode to print	[-|l|d] rwxrwxrwx 
   //use ip->OTHER FIELDs to print:  link  gid  uid  size  date  name

	//Look at INODE Struct def for types
	//Permissions
	printf("");
	//Link count - how many references to the file
	//Group ID
	//USER ID
	//SIZE - # of bytes
	//Creation time and date using ctime();
	//name

	//Check if link file
	//Softlink: file that points to another space, different INodes
	//Hardlink: different name for the same inode (file), 
		//Same physical space
   if (S_ISLINK(ip->i_mode)) //Just for softlinks
	//lrwxrwxrwx  1 lorinv lorinv   10 Apr  9 12:38 foo -> test.txt
	  printf(" -> %s\n", (char *)ip->i_block); //
}

int list_dir(mip)
{
   MINODE *cip;
   INODE *ip = &mip->INODE;
   //for each DIR entry in (data blocks of) *ip do{
	//Grab from my show block
	//Loops through everything in that one dir
	while ()
	{	
		//not part of actuall LS
	   // print [ino rec_len name_len name]
		//rec_len = total length of one entry for one file
		//print out all the entried in the dir struct.:)
		//in my show block :)
		
	
	   cip = iget(dev, ino);

		//Prints ls information for all the files
	   list_file(cip, name);

	   iput(cip);
   }
} 
*/
		
/*
QUESTIONS:
What does getino(int dev, char *pathname) do? :
	- Gets the Inode # based on path
	- dev = device you are reading from
		- dev = fd = open("device", O_RDONLY);
	- pathname = string of path name to file or dir
iget(dev, ino)? : Gets the inodes from the disk and puts it into memor
	
iput(MINODE *mip)? :
	- Gets the inodes from memory and puts it back on the disk.
	- mip = the inode you've been working with in memory
get_block(fd, blk, buf):
	- fd - file descriptor
	- blk - Everything on your HDD is saved in a block
	- Multiple inodes are stored in one block.
	- Inode represent a file.
*/


cd (char *pathname)
{
	MINODE *mip;
	int ino;
	if(pathname == "")
	{//cd to root;
		//You know the ino and device
		//The ino for root is 2 
		//dev is the global device
		//Running is the current process we are working with
		//Either P0 or P1
		//Everytime you use iget you need an iput
		//Why? Changes you made to that inode won't be retained.
		mip = iget(dev, 2);
		running->cwd = mip;
	}
	else
	{
		//Strtok will destroy the pathname, so make copy
		ino = getino(dev, pathname);
		mip = iget(dev, ino);
		//VERIFY mip->INODE is a DIR;
		//Check mip->inode->i_mode - bitmask to check if DIR
		//Because you can't CD into a regular file
		if (S_ISDIR(mip->INODE.i_mode)) 
		{
			running->cwd = mip;
		}
		else
		{
			printf("You suck\n");
		}
	}

	//Puts the inodes back on the disk
	iput(mip); 
}


//pwd command: print the pathname of running PROC's CWD

/*
stat()
{  // THIS IS TRIVIAL, hence no need to implement
	//Copy from mip to the stat stuct
	//Then print?
	 ino = getino(dev, pathname);
	 mip = iget(dev, ino);
	 copy info from minode.INODE to a stat structure.
}		 
*/

/****************** FINAL FORM OF main() **************************/

char line[128], pathname[64], parameter[64];

int main(int argc, char *argv[ ]) 
{
	int  Index; 
	char cmd[32];
	char *commands[10] = {"ls", "cd", "pwd"};
	void (*functions[4]) (char *pathname);
	//functions[0] = ls;
	functions[1] = cd;
	//functions[3] = pwd;
	
	char cname[30], pathname[100], parameter[50];	

	init();
	mount_root(argv[1]);

	while(1){
		//printf("P%d running: ", running->pid);
		printf("input command : ");
		fgets(line, 128, stdin);
		line[strlen(line)-1] = 0;  // kill the \n char at end
		if (line[0]==0) continue;

		sscanf(line, "%s %s %64c", cname, pathname, parameter);
		int i = 0;
		for (; i < 3; i++)
		{
			if(strcmp(cname, commands[i]) == 0)
			{
				functions[i](pathname);
				break;
			}
		}	
		
/*
		Index = findCmd(cmd); // map cmd to an index

		switch(Index){
		   ------------ LEVEL 1 -------------------
		   case 0 : menu();					  break;
		   case 1 : list_dir();				  break;
		   case 2 : change_dir();			  break;
		   case 3 : pwd(cwd);				  break;
		   case 4 : mkdir();				  break;
		   case 5 : rmdir();				  break;
		   case 6 : creat();				  break;
		   case 7 : link();					  break;
		   case 8 : unlink();				  break;
		   case 9 : symlink();				  break;
		   case 10: rm();					  break;
		   case 11: chmod();				  break;
		   case 12: chown();				  break;
		   case 13: stat();					  break;
		   case 14: touch();				  break;

		   -------------- LEVEL 2 ------------------
		   case 20: open();					  break;
		   case 21: close();				  break;
		   case 22: pfd();					  break;
		   case 23: lseek();				  break;
		   case 24: access();				  break;
		   case 25: read();					  break;
		   case 26: write();				  break;
		   case 27: cat();					  break;
		   case 28: cp();					  break;
		   case 29: mv();					  break;

		   ------------- LEVEL 3 -------------------
		   case 30: mount();				  break;
		   case 31: umount();				  break;
		   case 32: cs();	//switch process  break;
				 
		   case 40: sync(); //sync FS		  break; 
		   case 41: quit();					  break; 
		   default: printf("invalid command\n");
		 }
	}
} /* end main
***************************************************************************

 INSTEAD OF A switch-case table, you MUST use a Table of Function Pointers

*/

}

}
