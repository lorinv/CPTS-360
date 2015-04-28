#include "type.h"
=====================================================================
ASSUME: pathname = /a/b/c  OR  a/b/c

  int dev, ino;
  if (pathname[0]=='/')
    dev = root->dev;            // start from / 
  else
    dev = running->cwd->dev;    // start from running's CWD

ino = getino(&dev, pathname);  // modify *dev whenever dev changes
=====================================================================

int getino(int *dev, char *pathname) 
{
  // WRITE YOUR OWN getino() code; 
  // MODIFY *dev whenever dev changes, i.e.when cross mounting points
}   
