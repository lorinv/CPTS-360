/*
Lorin Vandegrift
11354621
This file will open and close a file given a file name or descriptor.
*/

#include <stdio.h>

//External Variables
extern PROC* running;
extern MINODE *root;
extern int extFD;

//Closes the current open file
int close_file()
{
	int fd = extFD;
	OFT* oftp;
	MINODE *mip;

	//Verify fd is within range
	if (fd > NFD || fd == -1)
		return -1;
	//verify running->fd[fd] is pointing at a OFT entry
	if (running->fd[fd] == 0) return;

	oftp = running->fd[fd];
	running->fd[fd] = 0;
	oftp->refCount--;
	if (oftp->refCount > 0) return 0;

	//last user of this OFT entry ==> displose of the MINODE[]
	mip = oftp->inodeptr;
	iput(mip);

	return 0;
}


// Opens a file when the user inputs a filename and a mode
// The file is then then saved to the OFT and the running
// process
int open_file()
{
	char buf[1024];
	int *dev;
	int i, ino, numMode;
	MINODE *mip;
	OFT* oftp;
	char path[128], mode[6];

	//Get the pathname from the user
	printf("Enter pathname: ");
	fgets(path, 128, stdin);
	path[strlen(path) - 1] = 0;

	//Ask for the pathname and mode to open
	printf("Input mode to open (0|1|2|3 for RD|WR|RW|APPEND):");
	fgets(mode, 6, stdin); 
	numMode = atoi(mode);
	if (numMode < 0 || numMode > 3)
	{
		printf("Invalid Mode.\n");
		return;
	}

	//Check if the pathname is relative or from root 
	//Get the device, which refers the FS you are working with
	if (path[0] == '/')
	{
		dev = root->dev;	
	}
	else
	{
		dev = running->cwd->dev;
	}

	//Gets and stores the ino and mip
	ino = getino(&dev, path);
	printf("ino = %d\n", ino);
	mip = iget(dev, ino);
	
	
	printf("Permissions in INODE: %o\n", mip->INODE.i_mode); 

	//Use the i_mode to verify it's a REGULAR file	
	if(S_ISDIR(mip->INODE.i_mode))
	{
		printf("Invalid file type\n");
		iput(mip);
		return;
	}
				
	//Check permissions
	//User selects Read and user can read
	if ((numMode == 0 && !((mip->INODE).i_mode & 0b1100100100)) ||
		//User selects Write and file can be writen to
		(numMode == 1 && !((mip->INODE).i_mode & 0b1010100100)) ||
		//User selects RW and is available
		(numMode == 2 && !((mip->INODE).i_mode & 0b1110100100)) ||
		//User selects APPEND
		(numMode == 3 && !((mip->INODE).i_mode & 0b1110100100)))
	{
		printf("Wrong permissions set.\n");
		iput(mip);
		return;
	}
	

	//Check if file already opened with INCOMPATABLE mode
	//(Only multiple reads are okay, nothing else.)
	//Number of file descriptors
    for (i = 0; i < NFD; i++)
	{
		//If the file descriptor isn't null
		if(running->fd[i])
		{
			//open file table for process
			oftp = running->fd[i];
			//If there is at least one file open and
			//The file we are trying to open is not already open
			if(oftp->refCount && oftp->inodeptr == mip)
			{
				if (oftp->mode)
				{
					printf("File already opened - incompatible mode.\n");
					iput(mip);
					return;
				}
				break;
			}		
		}
		oftp = 0;
	}			

	//Allocate a FREE OpenFileTable (OFT) and fill values
	//Keeps track of the connections to the file
	//Each connection has a connection status such as read/write
	if (oftp && oftp->mode == numMode)
	{
		oftp->refCount++;
	}
	else
	{
		
		for (i = 0; i < NOFT; i++)
		{
			if (oft[i].refCount == 0)
			{
				oftp = &(oft[i]);
				break;
			}
		}

		if (oftp == 0)
		{
			//If oft is full
			iput(mip);
			return;
		}
		oftp->mode = numMode; //open mode?
		oftp->refCount = 1;
		oftp->inodeptr = mip;
	}	

	//Depending on open mode, set the OFT's offset 
	switch (numMode)
	{
		case 0:
			oftp->offset = 0;
			break;
		case 1:
			truncate(mip); //WR: truncate file to 0 size
			oftp->offset = 0;
			break;
		case 2:
			oftp->offset = 0; //RW does not truncate file
			break;
		case 3:
			oftp->offset = mip->INODE.i_size; //APPEND mode
			break;
		default:
			printf("Invalid Mode\n");
			return -1;
	}

	//Find smallest i in running PROC ...
	for (i = 0; i < NFD; i++)
	{
		//Smallest i that NULL
		if (running->fd[i] == 0)
		{
			running->fd[i] = oftp;
			break;
		}
	}

	//Update INODE's time field
	//mark MINODE as dirty
	//Applies for all but reading
	if (1 == numMode || 2 == mode || 3 == mode)
	{
		//ctime = change time
		//mtime = modification time
		//atime = access time
		mip->INODE.i_atime = time(0L);
		mip->INODE.i_mtime = time(0L);
		mip->dirty = 1;
	}

	//Return i as the file descriptor
	extFD = i;
	running->cwd = mip;
	printf("%s was successfully opened.\n", path);
}


////////////////////////////


// Opens a file when the user inputs a filename and a mode
// The file is then then saved to the OFT and the running
// process
int open_file2(char *path, int mode)
{
	char buf[1024];
	int *dev;
	int i, ino, numMode;
	MINODE *mip;
	OFT* oftp;

	//path[strlen(path) - 1] = 0;

	//Ask for the pathname and mode to open
	numMode = mode;
	if (numMode < 0 || numMode > 3)
	{
		printf("Invalid Mode.\n");
		return;
	}

	//Check if the pathname is relative or from root 
	//Get the device, which refers the FS you are working with
	if (path[0] == '/')
	{
		dev = root->dev;	
	}
	else
	{
		dev = running->cwd->dev;
	}

	//Gets and stores the ino and mip
	ino = getino(&dev, path);
	printf("ino = %d\n", ino);
	mip = iget(dev, ino);
	
	
	printf("Permissions in INODE: %o\n", mip->INODE.i_mode); 

	//Use the i_mode to verify it's a REGULAR file	
	if(S_ISDIR(mip->INODE.i_mode))
	{
		printf("Invalid file type\n");
		iput(mip);
		return;
	}
				
	//Check permissions
	//User selects Read and user can read
	if ((numMode == 0 && !((mip->INODE).i_mode & 0b1100100100)) ||
		//User selects Write and file can be writen to
		(numMode == 1 && !((mip->INODE).i_mode & 0b1010100100)) ||
		//User selects RW and is available
		(numMode == 2 && !((mip->INODE).i_mode & 0b1110100100)) ||
		//User selects APPEND
		(numMode == 3 && !((mip->INODE).i_mode & 0b1110100100)))
	{
		printf("Wrong permissions set.\n");
		iput(mip);
		return;
	}
	

	//Check if file already opened with INCOMPATABLE mode
	//(Only multiple reads are okay, nothing else.)
	//Number of file descriptors
    for (i = 0; i < NFD; i++)
	{
		//If the file descriptor isn't null
		if(running->fd[i])
		{
			//open file table for process
			oftp = running->fd[i];
			//If there is at least one file open and
			//The file we are trying to open is not already open
			if(oftp->refCount && oftp->inodeptr == mip)
			{
				if (oftp->mode)
				{
					printf("File already opened - incompatible mode.\n");
					iput(mip);
					return;
				}
				break;
			}		
		}
		oftp = 0;
	}			

	//Allocate a FREE OpenFileTable (OFT) and fill values
	//Keeps track of the connections to the file
	//Each connection has a connection status such as read/write
	//TODO
	if (oftp && oftp->mode == numMode)
	{
		oftp->refCount++;
	}
	else
	{
		
		for (i = 0; i < NOFT; i++)
		{
			if (oft[i].refCount == 0)
			{
				oftp = &(oft[i]);
				break;
			}
		}

		if (oftp == 0)
		{
			//If oft is full
			iput(mip);
			return;
		}
		oftp->mode = numMode; //open mode?
		oftp->refCount = 1;
		oftp->inodeptr = mip;
	}	

	//Depending on open mode, set the OFT's offset 
	switch (numMode)
	{
		case 0:
			oftp->offset = 0;
			break;
		case 1:
			truncate(mip); //WR: truncate file to 0 size
			oftp->offset = 0;
			break;
		case 2:
			oftp->offset = 0; //RW does not truncate file
			break;
		case 3:
			oftp->offset = mip->INODE.i_size; //APPEND mode
			break;
		default:
			printf("Invalid Mode\n");
			return -1;
	}

	//Find smallest i in running PROC ...
	for (i = 0; i < NFD; i++)
	{
		//Smallest i that NULL
		if (running->fd[i] == 0)
		{
			running->fd[i] = oftp;
			break;
		}
	}

	//Update INODE's time field
	//mark MINODE as dirty
	//Applies for all but reading
	if (1 == numMode || 2 == mode || 3 == mode)
	{
		//ctime = change time
		//mtime = modification time
		//atime = access time
		mip->INODE.i_atime = time(0L);
		mip->INODE.i_mtime = time(0L);
		mip->dirty = 1;
	}

	//Return i as the file descriptor
	//printf("%s was successfully opened.\n", path);
	return i;
}
