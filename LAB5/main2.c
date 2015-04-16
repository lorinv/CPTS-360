//mkdir and symlink are similar (like copy paste with slight changes)
//Same for rmdir and unlink (a few more differences)

360 HOW TO mkdir_creat

   
            360 File System: HOW TO mkdir-creat

Assume: command line = "mkdir pathname" OR  "creat pathname"
Extract cmd, pathname from line and save them as globals.

make_dir()
{
1. pahtname = "a/b/c" start mip = root;
            = "a/b/c" start mip = running->cwd;

2. Let  
     parent = dirname(pathname);   parent= "/a/b" OR "a/b"
     child  = basename(pathname);  child = "c"

3. Get the In_MEMORY minode of parent:

         pino  = getino(dev, parent);
         pip   = iget(dev, pino); 

   Verify : (1). parent INODE is a DIR (HOW?)   AND
            (2). child does NOT exists in the parent directory (HOW?);
               
4. call mymkdir(pip, child);

5. increment parent inodes's link count by 1; 
   touch its atime and mark it DIRTY //DIRTY is a flag that tells you
									//if the inode had been modified
	//atime is a value in inode - last time accessed
	//ctime - created time
	INODE.i_atime = time(0L); //time gets you the time, OL gets you
								//The current system time.

	//Use refcount to tell if someone is accessing inode

6. iput(pip);
     
} 


int mymkdir(MINODE *pip, char *name)
{
1. pip points at the parent minode[] of "/a/b", name is a string "c") 

2. allocate an inode and a disk block for the new directory;
        ino = ialloc(dev);    
        bno = balloc(dev);

3. mip = iget(dev, ino) to load the inode into a minode[] (in order to
   wirte contents to the INODE in memory).

4. Write contents to mip->INODE to make it as a DIR.

5. iput(mip); which should write the new INODE out to disk.

  // C CODE of (3), (4) and (5):
  //**********************************************************************
  mip = iget(dev,ino);
  INODE *ip = &mip->INODE;
  Use ip-> to acess the INODE fields:

  i_mode = 0x41ED;		// OR 040755: DIR type and permissions
  i_uid  = running->uid;	// Owner uid 
  i_gid  = running->gid;	// Group Id
  i_size = BLKSIZE;		// Size in bytes 
  i_links_count = 2;	        // Links count=2 because of . and ..
  i_atime = i_ctime = i_mtime = time(0L);  // set to current time
  i_blocks = 2;					// LINUX: Blocks count in 512-byte chunks 
  i_block[0] = bno;             // new DIR has one data block   
  i_block1] to i_block[14] = 0;
 
  mip->dirty = 1;               // mark minode dirty
  iput(mip);                    // write INODE to disk


//***** create data block for new DIR containing . and .. entries ******
6. Write . and .. entries into a buf[ ] of BLKSIZE

   | entry .     | entry ..                                             |
   ----------------------------------------------------------------------
   |ino|12|1|.   |pino|1012|2|..                                        |
   ----------------------------------------------------------------------

   Then, write buf[ ] to the disk block bno;

7. Finally, enter name ENTRY into parent's directory by 
            enter_name(pip, ino, name);


8. int enter_name(MINODE *pip, int myino, char *myname)
{
 For each data block of parent DIR do { // assume: only 12 direct blocks

     if (i_block[i]==0) BREAK;

(1). get parent's data block into a buf[];
   
(2). EXT2 DIR entries: Each DIR entry has rec_len and name_len. Each entry's
     ideal length is   IEAL_LEN = 4*(8 + name_len + 3)/4). 
     All DIR entries in a data block have rec_len = IDEAL_LEN, except the last
     entry. The rec_len of the LAST entry is to the end of the block, which may
     be larger than its IDEAL_LEN.

  --|-4---2----2--|----|---------|--------- rlen ->------------------------|
    |ino rlen nlen NAME|.........|ino rlen nlen|NAME                       |
  --------------------------------------------------------------------------

(3). To enter a new entry of name with n_len, the needed length is
        need_length = 4*((8 + n_len + 3)/4)  // a multiple of 4

(4). Step to the last entry in a data block (HOW?).

 
    // get parent's ith data block into a buf[ ] 

       get_block(parent->dev, parent->INODE.i_block[i], buf);
  
       dp = (DIR *)buf;
       cp = buf;

       // step to LAST entry in block: int blk = parent->INODE.i_block[i];
       printf("step to LAST entry in data block %d\n", blk);
       while (cp + dp->rec_len < buf + BLKSIZE){

          /****** Technique for printing, compare, etc.******
          c = dp->name[dp->name_len];
          dp->name[dp->name_len] = 0;
          printf("%s ", dp->name);
          dp->name[dp->name_len] = c;
          **************************************************/

          cp += dp->rec_len;
          dp = (DIR *)cp;
       } 
       // dp NOW points at last entry in block
  

     Let remain = LAST entry's rec_len - its IDEAL_LENGTH;

     if (remain >= need_length){
        enter the new entry as the LAST entry and trim the previous entry
        to its IDEAL_LENGTH; 
        goto (6) below.
     } 

                          EXAMPLE:

                                 |LAST entry 
  --|-4---2----2--|----|---------|--------- rlen ->------------------------|
    |ino rlen nlen NAME|.........|ino rlen nlen|NAME                       |
  --------------------------------------------------------------------------
                                                    |     NEW entry
  --|-4---2----2--|----|---------|----ideal_len-----|--- rlen=remain ------|
    |ino rlen nlen NAME|.........|ino rlen nlen|NAME|myino rlen nlen myname|
  --------------------------------------------------------------------------

}

(5).// Reach here means: NO space in existing data block(s)

  Allocate a new data block; INC parent's isze by 1024;
  Enter new entry as the first entry in the new data block with rec_len=BLKSIZE.

  |-------------------- rlen = BLKSIZE -------------------------------------
  |myino rlen nlen myname                                                  |
  --------------------------------------------------------------------------

(6).Write data block to disk;
}        


creat_file()
{
  This is ALMOST THE SAME AS mkdir() except : 
   (1). its inode's mode field is set to a REGULAR file, 
        permission bits to (default) rw-r--r--, 
   (2). No data block, so size = 0
   (3). links_count = 1;
   (4). Do NOT increment parent's links_count
} 


int my_creat(MINODE *pip; char *name)
{
  Same as mymkdir() except 
    INODE's file type = 0x81A4 OR 0100644
    links_count = 1
    NO data block, so size = 0
    do NOT inc parent's link count.
}  

====================================================================

================ development HEPS =============================

1. Your mkdir/creat may trash the disk iamge (by writing to wrong inode or data
   blocks), which will cause problems later even if your program is correct. So,
   it's better to use a FRESH disk image each time during development.

   Write a sh script "run" or "doit" containing: 

         mkfs disk 1440  # renew the disk image file
         a.out

   Enter run or doit to test YOUR new a.out, so that you use a NEW disk image 
   file each time until YOUR a.out no longer traches the disk image.

2. After running YOUR mkdir/creat commands, it's better to check the results 
   under LINUX. Write a sh script "s" containing
       
         mount -o loop disk /mnt
         ls -l /mnt
         umount /mnt

   so that s will show the disk contents under LINUX.
==============================================================
  

     
