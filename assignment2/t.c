#include <stdio.h>
#include <stdlib.h>

int *ebp;

main(int argc, char *argv[ ], char *env[ ])
{
  int a,b,c;
  printf("enter main\n");
  
  printf("&argc=%8x argv=%8x env=%8x\n", &argc, argv, env);
  printf("&a=%8x    &b=%8x   &c=%8x\n",  &a, &b, &c);

  a=1; b=2; c=3;
  A(a,b);
  printf("exit main\n");
}

int A(int x, int y)
{
  int d,e,f;
  printf("enter A\n");
  // PRINT ADDRESS OF d, e, f
  printf("&d=%8x, &e=%8x, &f=%8x\n", &d, &e, &f);

  d=3; e=4; f=5;
  B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("enter B\n");
  // PRINT ADDRESS OF g,h,i
  printf("&g=%8x, &h=%8x, &i=%8x\n", &g, &h, &i);

  g=6; h=7; i=8;
  C(g,h);
  printf("exit B\n");
}

int C(int x, int y)
{
  int i;
  int u, v, w, *p;
  printf("enter C\n");
  // PRINT ADDRESS OF u,v,w;
  printf("&u=%8x, &v=%8x, &w=%8x\n", &u, &v, &w); 
 
  u=9; v=10; w=11;

  ebp = (int *)get_ebp();
  
  int *temp = ebp;

  printf("&ebp= %8x -> ", temp);
  temp = *temp;

  while(*temp)
  {
    printf("%8x ->", temp);
    temp = *temp; //Move to the next item in the linked list
  }
  printf("%8x\n", temp);
  
  *p = p;

  printf("\nAddress\t\tContents\t\t\n\n");
  for (i=0; i<100; i++)
  {
    printf("%8x\t%8x\n", p, *p);
    p++;
  }

  printf("exit C\n");
}


  

