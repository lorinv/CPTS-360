#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define MAX 10000
typedef struct {
    char *name;
    char *value;
} ENTRY;

char buffer[4096];
ENTRY entry[MAX];

char * checkRelPath (char * cwd, char * value)
{
	char * tempDir = cwd;	

	//Check if relative to root
	if (value[0] == '/')
	{
		tempDir = value;
	}
	// Path is relative to current dir	    
	else
	{
		strcat(tempDir,"/");
		strcat(temp, value);
	}

	return tempDir;
}

main(int argc, char *argv[]) 
{
  int i, m, r;
  char cwd[128];

  //Variables I added
  char * tempDir = cwd;
  char * tempDir2 = cwd;
  FILE * readFile;
  FILE * readFile2;
  char readChar;
  int check;

  m = getinputs();    // get user inputs name=value into entry[ ]
  getcwd(cwd, 128);   // get CWD pathname

  printf("Content-type: text/html\n\n");
  printf("<p>pid=%d uid=%d cwd=%s\n", getpid(), getuid(), cwd);

  printf("<H1>Echo Your Inputs</H1>");
  printf("You submitted the following name/value pairs:<p>");
 
  for(i=0; i <= m; i++)
     printf("%s = %s<p>", entry[i].name, entry[i].value);
  printf("<p>");

  //Perform mkdir command
  if !strcmp(entry[0].name, "mkdir")
  {	
	//Check if relative to root
	tempDir = checkRelPath (cwd, entry[1].value)
	mkdir(tempDir,0755);
  }

  //Perform rmdir command
  else if !strcmp(entry[0].name, "rmdir")
  {
	//Check if relative to root
	tempDir = checkRelPath (cwd, entry[1].value)
    rmdir(tempDir);
  }

  //Perform rm command
  else if !strcmp(entry[0].name, "rm")
  {
	//Check if relative to root
	tempDir = checkRelPath (cwd, entry[1].value)
	unlink(tempDir);
  }
 
  //Perform cat command
  else if !strcmp(entry[0].name, "cat")
  {
	//Prepare file to display
	tempDir = checkRelPath (cwd, entry[1].value)
	readFile = fopen(tempDir, "r");
	if (readFile != NULL)
	{
		readChar = fgetc(readFile);
		while (readChar != EOF)
f		{
			//Check for new line
			if (readChar == '\n')
				printf("<br>");
			else
				printf("%c", readChar);
			readChar = fgetc(readFile);
		}
	}
	fclose(readFile);
  }

  //Perform cp command
  else if !strcmp(entry[0].name, "cp")
  {
	//Check if relative to root
	tempDir = checkRelPath (cwd, entry[1].value)
	tempDir2 = checkRelPath (cwd, entry[2].value)
	readFile = fopen(tempDir, "r");
	readFile2 = fopen(tempDir2, "w");
	if (readFile != NULL && readFile2 != NULL)
	{
	  check = fread(buffer, 1, 4096, readFile)
	  while(check)
	  {
	      fwrite(buffer, 1, check, readFile2);
	      total += check;
		  check = fread(buffer, 1, 4096, readFile)
	  }
	}
	fclose (readFile);
	fclose (readFile2);
  }

  //Perform ls command
  else if !strcmp(entry[0].name, "ls")
  {
	//Check if relative to root
	tempDir = checkRelPath (cwd, entry[1].value)
    DIR *dirHandle = NULL;
	struct dirent *dptr = NULL;
	dirHandle = opendir(tempDir);
	if (dirHandle != NULL)
	{
		dptr = readdir(dirHandle);
		for(i=0; dptr != NULL; i++)
		{
			printf("%s ",dptr->d_name);
			dptr = readdir(dirHandle);
		}
	    printf("<br>");
	}
	
  }



  /*****************************************************************
   Write YOUR C code here to processs the command
         mkdir dirname
         rmdir dirname
         rm    filename
         cat   filename
         cp    file1 file2
         ls    [dirname] <== ls CWD if no dirname
  *****************************************************************/
 
  // create a FORM webpage for user to submit again 
  printf("</title>");
  printf("</head>");
  printf("<body bgcolor=\"#FF0000\" link=\"#330033\" leftmargin=8 topmargin=8");
  printf("<p>------------------ DO IT AGAIN ----------------\n");
  
  printf("<FORM METHOD=\"POST\" ACTION=\"http://cs560.eecs.wsu.edu/~vandegrift/cgi-bin/mycgi\">");

  //------ NOTE : CHANGE ACTION to YOUR login name ----------------------------
  //printf("<FORM METHOD=\"POST\" ACTION=\"http://cs560.eecs.wsu.edu/~YOURNAME/cgi-bin/mycgi\">");
  
  printf("Enter command : <INPUT NAME=\"command\"> <P>");
  printf("Enter filename1: <INPUT NAME=\"filename1\"> <P>");
  printf("Enter filename2: <INPUT NAME=\"filename2\"> <P>");
  printf("Submit command: <INPUT TYPE=\"submit\" VALUE=\"Click to Submit\"><P>");
  printf("</form>");
  printf("------------------------------------------------<p>");

  printf("</body>");
  printf("</html>");
}
