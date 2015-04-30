#ifndef __UTIL__
#define __UTIL__

#include "../type.h"

#define BLKSIZE 1024

PROC* running;
MINODE *root;
MINODE minode[NMINODES];

typedef unsigned int   u32;

// define shorter TYPES, save typing efforts
typedef struct ext2_group_desc	GD;
typedef struct ext2_super_block SUPER;
typedef struct ext2_inode		INODE;
typedef struct ext2_dir_entry_2 DIR;	

int init();
int getino (int* dev, char* pathname);
int get_block(int fd, int blk, char *buf);
int put_block(int dev, int blk, char *buf);
int token_path(char *pathname, char **token_ptrs);
char* dir_name(char* pathname);
char* base_name(char* pathname);
int search (MINODE *inodePtr, char *name);
MINODE* iget (int dev, u32 ino);
int iput (MINODE *mip);
int findmyname(MINODE *parent, int myino, char *myname);
int findino(MINODE *mip, int *myino, int *parentino);
int ialloc(int dev);
int balloc(int dev);
int idealloc(int dev, int ino);
int bdealloc(int dev, int bno);

#endif
