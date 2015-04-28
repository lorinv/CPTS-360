#define BUSY    1
#define FREE    0
#define NCMDS   31

/* command table */

char *cmds[] = {"mkdir",  "cd",    "pwd",    "ls",     "mount",
                "umount", "creat", "rmdir",  "rm",     "open",
                "close",  "read",  "write",  "cat",    "cp",
                "mv",     "pfd",   "lseek",  "rewind", "stat", 
                "pm",     "menu",  "access", "chmod",  "chown", 
                "cs",     "fork",  "ps",     "kill",   "quit",
                "touch",  "sync",  "link",   "unlink", "symlink", 0  
               };


MINODE *root, *cwd;
char pathname[128], parameter[128], *name[128], cwdname[128];
char names[128][256];

int  nnames;
char *rootdev = "/dev/fd0", *slash = "/", *dot = ".";
int iblock;

MINODE minode[NMINODES];
MOUNT  mounttab[NMOUNT];
PROC   proc[NPROC], *running;
OFT    oft[NOFT];

//char *basename(), *dirname();

MOUNT *getmountp();

int DEBUG=0;
int nproc=0;

extern int show();


int findCmd(s) char *s;
{
  int i;
  for (i=0; cmds[i]; i++)
      if ( strcmp(cmds[i], s) == 0) 
         return(i);
  return(-1);
}


int menu()
{
  printf("        ******************** Menu *******************\n");
  printf("        mkdir     creat     mount     umount    rmdir\n");
  printf("        cd        ls        pwd       stat      rm   \n");   
  printf("        link      unlink    symlink   chmod     chown  touch\n");
  printf("        open      pfd       lseek     rewind    close\n");    
  printf("        read      write     cat       cp        mv\n");
  printf("        cs        fork      ps        kill      quit\n");

  printf("        =============   Usage Examples ==============\n");
  printf("        mkdir  filename\n");
  printf("        mount  filesys   /mnt\n");
  printf("        chmod  filename  0644\n");
  printf("        chown  filename  uid\n");
  printf("        open   filename  mode (0|1|2|3 for R|W|RW|AP)\n");
  printf("        write  fd  text_string\n");
  printf("        read   fd  nbytes\n");
  printf("        pfd    (display opened file descriptors)\n");
  printf("        cs     (switch process)\n"); 
  printf("        fork   (fork child process)\n");
  printf("        ps     (show process queue as Pi[uid]==>}\n");
  printf("        kill   pid   (kill a process)\n");
  printf("        *********************************************\n");
}


int pm()
{
  int i;    MINODE *ip;
  printf("inode refCount\n");
  for (i=0; i<NMINODES; i++){
      ip = &minode[i]; 
      if (ip->refCount > 0){
         printf("[%d %d]    %d\n", ip->dev, ip->ino, ip->refCount);
      }
  }
}


 
