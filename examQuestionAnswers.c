#include <stdio.h>

//1.
/*
Given: sizeof (int) = 4; int buf(1024); 

1. How many bits are in buf[ ]: 
	- 1 byte = 8 bites
	- sizeOf returns the number of bytes in the given type
	- So a integer buffer with 1024 cells will take up:
		1024 * 4 * 8 = 32768
	
2. Assume bits are counted from 0. For BIT=321. Which buf[i] contains this BIT?:
	- How many bits for byte? 8
	- So 322 / 8 = 40.25 because it indexes from 0
	- How many bytes in an int? 4
	- 40.25 / 4 = ~10
	- So BIT=321 is in buf[10]

3. Write C code to test whether BIT=321 in buf[ ] is 0 or 1:
	- Numbers are stored in a series of bits, 0's or 1's
	- For Example - 5 is stored as 00000101 
	- 8 bites in a byte
	- 
*/
int checkBit (int num)
{
	int buf[1024];
	int index = (num + 1 / 8.0 / 4.0);
	int remainder = 322 - (index * 32);
	buf[10] |= 1 << remainder;
	int result = ((buf[index] >> remainder) & 1);
	printf("Bit at %d is %d\n.", num, result);
	return result;
}
/*

4. Assume bits in buf[] are counted from 300, i.e. 300, 301, 302
	Write C code to set BIT=321 in buf[] to 1:

5. Write C code to clear BIT=321 in buf[] to 0:

*/

//2.
/*
Assume: An INODE has SOME indirect blocks
Given: char buf[1024];
1. Write C code to read the indirect block of this INODE into buf[];
*/

int readIndirectBlock(fd, INODE *inodePointer)
{
	char buf[1024];
	get_block(fd, iNodePointer->i_block[12], buf);
	//lseek(fd, buf, 0)
	//read(fd, buf, BLKSIZE`
	int * tempPtr = buf;
	while (tempPtr != 0 && tempPtr < buf + 1024)
	{
		if (*tempPtr != 0)
		{
			printf(" %d", *tempPtr);
		}	
		tempPtr++;
	}
}

//Write C code to read the block containing thisInode into buf[]
int readInodeIntoBuf(int ino)
{
	char buf[1024];
	get_block(fd, 2, buf);
	GD * gd = buf;
	int InodesBeginBlock = gd[0].bg_inode_table;
	
	get_block(fd, InodesBeginBlock, buf);
	INODE * ip = (INODE *)buf + 1;
	int block = ((ino - 1) / 8) + InodesBeginBlock;	
	int index = (inumber - 1) % 8;
	get_block(fd, block, buf);
	ip = (INODE *)buf + index;	
}	

int main()
{
	checkBit (321);
}

