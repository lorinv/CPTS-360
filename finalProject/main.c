#include "type.h"
#include "global.c"

#include "projectFiles/open_close.c"
//#include "projectFiles/lseek.c"
#include "projectFiles/read.c"
#include "projectFiles/write.c"
#include "projectFiles/cp_mv.c"
//#include "projectFiles/mount_unmount.c"
#include "projectFiles/cat.c"
//#include "projectFiles/util.h"

FILE *fp;
int extFD;
/************** command table *****************************
char *cmds[] = {"mkdir",  "cd",    "pwd",    "ls",     "mount",
                "umount", "creat", "rmdir",  "rm",     "open",
                "close",  "read",  "write",  "cat",    "cp",
                "mv",     "pfd",   "lseek",  "rewind", "stat", 
                "pm",     "menu",  "access", "chmod",  "chown", 
                "cs",     "fork",  "ps",     "kill",   "quit",
                "touch",  "sync",  "link",   "unlink", "symlink", 0  
               };

int findCmd(char *s)
{
  int i;
  for (i=0; cmds[i]; i++)
      if ( strcmp(cmds[i], s) == 0) 
         return(i);
  return(-1);
}
**********************************************************/

int mountroot()   /* mount the root file system */
{
  int i, ino, fd, dev;
  MOUNT *mp;
  SUPER *sp;
  MINODE *ip;

  char line[64], buf[BLOCK_SIZE], *rootdev;
  int ninodes, nblocks, ifree, bfree;

  printf("enter rootdev name (RETURN for mydisk) : ");
  fgets(line,64,stdin);
  line[strlen(line)-1] = 0;

  rootdev = "mydisk";

  if (line[0] != 0)
     rootdev = line;

  dev = open(rootdev, O_RDWR);
  if (dev < 0){
     printf("panic : can't open root device\n");
     exit(1);
  }

  /* get super block of rootdev */
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;

  /* check magic number */
  if (sp->s_magic != SUPER_MAGIC){
     printf("super magic=%x : %s is not a valid Ext2 filesys\n",
             sp->s_magic, rootdev);
     exit(0);
  }
  mp = &mounttab[0];             /* use mounttab[0] */

  /* copy super block info into mounttab[0] */
  ninodes = mp->ninodes = sp->s_inodes_count;
  nblocks = mp->nblocks = sp->s_blocks_count;
  
  bfree = sp->s_free_blocks_count;
  ifree = sp->s_free_inodes_count;

  mp->dev = dev;         
  mp->busy = BUSY;

  strcpy(mp->name, rootdev);
  strcpy(mp->mount_name, "/");

  get_block(dev, 2, buf);
  gp = (GD *)buf;
  iblock = gp->bg_inode_table;

  printf("iblock = %d\n", iblock);
  printf("bmap=%d\n", gp->bg_block_bitmap);
  printf("imap=%d\n", gp->bg_inode_bitmap);

  /***** call iget(), which inc the Minode's refCount ****/

  root = iget(dev, 2);          /* get root inode */

  mp->mounted_inode = root;
  root->mountptr = mp;

  printf("mount : %s  mounted on / \n", rootdev);
  printf("nblocks=%d  bfree=%d   ninodes=%d  ifree=%d\n",
	  nblocks, bfree, ninodes, ifree);

  return(0);
} 

int init()
{
  int i,j;
  MINODE *rootip;
  PROC *p;

  for (i=0; i<NMINODES; i++)
      minode[i].refCount = 0;

  for (i=0; i<NMOUNT; i++)
      mounttab[i].busy = 0;

  for (i=0; i<NPROC; i++){
      proc[i].status = FREE;
      for (j=0; j<NFD; j++)
          proc[i].fd[j] = 0;
      proc[i].next = &proc[i+1];
  }

  for (i=0; i<NOFT; i++)
      oft[i].refCount = 0;

  printf("mounting root\n");
    mountroot();
  printf("mounted root\n");

  printf("creating P0, P1\n");
  p = running = &proc[0];
  p->status = BUSY;
  p->uid = 0; 
  p->pid = p->ppid = p->gid = 0;
  p->parent = p->sibling = p;
  p->child = 0;
  p->cwd = root;
  cwd = running->cwd;

  p = &proc[1];
  p->next = &proc[0];
  p->status = BUSY;
  p->uid = 2; 
  p->pid = 1;
  p->ppid = p->gid = 0;
  p->cwd = root;
  p->cwd->refCount++;

  nproc = 2;
}


int quit()
{
   int i;

   if (running->uid != 0){
       printf("not SUPER user\n");
       return(-1);
   }

   for (i=0; i<NMINODES; i++){
       if (minode[i].refCount){
           minode[i].refCount = 1;
           iput(&minode[i]);

           if (minode[i].mounted)
               minode[i].refCount = 1;
            else
             minode[i].refCount = 0; 
       }
   }

   for (i=NMOUNT-1; i>=0; i--){
        if (mounttab[i].busy){
            printf("umount %s\n",mounttab[i].name);
            umount(mounttab[i].name);
        }
   }

   exit(0);
} 


int sync()
{
  // write in-core INODEs to disk
  int i, save;
  MINODE *mip;
  printf("flush in-core inodes to disk .....\n");
  for (i=0; i<NMINODES; i++){
    mip = &minode[i];
    if (mip->refCount > 0){
       printf("flush minode[%d] : [%d %d]  %s\n", 
                           i, mip->dev, mip->ino, mip->name); 
       save = mip->refCount;
       mip->refCount = 1;
       iput(mip);
       mip->refCount = save;
    }
  }
}


int main(int argc, char *argv[ ]) 
{
  int i,cmd; 
  char line[128], cname[64];

  if (argc>1){
    if (strcmp(argv[1], "-d")==0)
        DEBUG = 1;
  }

  init();

  while(1){
      printf("P%d running: ", running->pid);

      /* zero out pathname, parameter */
      for (i=0; i<64; i++){
          pathname[i]=parameter[i] = 0;
      }      
      /* these do the same; set the strings to 0 */
      memset(pathname, 0, 64);
      memset(parameter,0, 64);
      
      if (running->status == KILLED)
         exit(0);

      printf("input command : ");
      fgets(line, 128, stdin);
      line[strlen(line)-1] = 0;
      if (line[0]==0) continue;

      sscanf(line, "%s %s %64c", cname, pathname, parameter);

      cmd = findCmd(cname);

      // REPLACE THIS TABLE WITH FUNCTION POINTERs
      switch(cmd){

           case 0 : make_dir();               break;
           case 1 : change_dir();             break;
           case 2 : pwd(cwd);                 break;
           case 3 : list_dir();               break;
           case 4 : mount();                  break;
  //         case 5 : umount(pathname);         break;
           case 6 : creat_file();             break;
           case 7 : rmdir();                  break;
           case 8 : rm_file();                break;
           case 9 : open_file();              break;
           case 10: close_file();             break;

           case 11: read_file();              break;
           case 12: write_file();             break;
           case 13: cat_file();               break;

           case 14: cp_file();                break;
           case 15: mv_file();                break;

      //     case 16: pfd();                    break;
   //       case 17: lseek_file();             break;
      //     case 18: rewind_file();            break;      
           case 19: mystat();                 break;

           case 20: pm();                     break;

           case 21: menu();                   break;

           case 22: access_file();            break;
           case 23: chmod_file();             break;
           case 24: chown_file();             break;

     //      case 25: cs();                     break;
      
           case 29: quit();                   break; 
           case 30: do_touch();               break;

           case 31: sync();                   break;
           case 32: link();                   break;
           case 33: unlink();                 break;
           case 34: symlink();                break;
           default: printf("invalid command\n");
                    break;

      }
  }
}







