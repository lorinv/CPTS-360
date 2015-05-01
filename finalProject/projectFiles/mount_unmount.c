//NEEDS TO BE TESTED and probably fixed

#include <stdio.h>

int mount()
{
	int i, fd, ino, dev;
	MINODE *mip;
	MOUNT *mntptr;
	char buf[BLKSIZE];
	SUPER * ext;

	//If no parameters were provided 
	if (strcmp(pathname, "") == 0)
	{
		//Display current mounted filesystems
		//NMOUNT - # of systems mounted
		printf("Files systems that are currently mounted: \n");
		for (i = 0; i < NMOUNT; i++)
		{
			if (mounttab[i].dev)
			{
				printf("\t%s\t%s\n", mounttab[i].name, mounttab[i].mount_name);
			}
		}
		return 0;
	}

	//Check is filesystem is already mounted
	//Search through the mount array
	for (i = 0; i < NMOUNT; i++)
	{
		//If already mounted, reject it
		if (mounttab[i].dev && !strcmp(mounttab[i].name, pathname))
		{
			printf("This filesystem is already mounted.\n");
			return;
		}
	}
		// Allocate a free MOUNT table entry
		for (i = 0; i < NMOUNT; i++)
		{
			if (mounttab[i].dev == 0)
			{	
				//mounttab[i].dev = dev;
				mntptr = &(mounttab[i]);
			}	
		}
		//open FS for RW, fd = new dev
		//Check if EXT file system, if not, reject
		fd = open(pathname, O_RDWR);
		if (fd == -1)
		{
			printf("Invalid file descriptor.\n");
			return -1;
		}
		dev = fd;
		
		get_block(fd, 1, buf);
		ext = (SUPER*)buf;
		if (ext->s_magic != 0xEF53)
		{
			printf("Not a EXT2 file system!\n");
			return;	
		} 
		
		ino = getino(&dev, pathname);
		mip = iget(dev, ino); //Load inode into memory
		
		//Check if mount point is a DIR
		if (!S_ISDIR(mip->INODE.i_mode))
		{
			printf("Invalid mount point...\n");
			return 0;
		}		

		//Check if mount_point is NOT busy (not someone's CWD)
		if (running->cwd->dev == mip->dev)
		{	
			printf("Directory is busy...\n");
			return 0;
		}

		//record new DEV in the MOUNT table entry
		mntptr->dev = dev;
		strcpy(mntptr->name, pathname);
		strcpy(mntptr->mount_name, parameter);
		mntptr->ninodes = ext->s_inodes_count;
		mntptr->nblocks = ext->s_blocks_count;
		mntptr->mounted_inode = iget(dev, ROOT_INODE);
		//TODO: Copy everything for safe keeping

		return 0;		
}

int umount (char *filesys)
{
	MOUNT* umnt = 0;

	if (strcmp(filesys, "") == 0)
	{
		printf("Please enter a FS to unmount\n");
		return -1;
	}

	int i, j, count = 0;;
	for (i = 0; i < NMOUNT; i++)
	{
		if(strcmp(mounttab[i].name, filesys) == 0)
		{
			count++;
			break;
		}
	}
	if (count == 0)
	{
		printf("%s is not mounted.\n", filesys);
		return -1;
	}

	//compare the cwd->dev to the MINODE->devs
	//If none match, no one is using it
	for (j = 0; j < NMINODES; j++)
	{
		if (minode[i].refCount && minode[i].mounted && 
			(minode[i].mountptr->dev == umnt->dev))
		{
			close(umnt->dev);
			minode[i].mountptr = 0;
			iput(&minode[i]);
			umnt->mounted_inode = 0;
			umnt->dev = 0;
			break;			
		}
	}
	
	//Get the inode using iget and the name passd
	//compare running->cwd to INODE mounttable
	//Look through the mount table
	//Make it not mounted
	//Put it back to the disk
	
	return 0;
}	

