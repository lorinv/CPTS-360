#include <stdio.h>
#include <stdarg.h>

void prints(char *s);

typedef unsigned int u32;

int main ()
{
	/*
	char *hi = "Hello";
	char *temp = hi;
	printf("%x\n", &hi);
	printf("%c\n", *hi);
	printf("%x\n", temp);
	printf("%c\n", temp[1]);
	*/
	//char * str = "Hello";
	//prints(str);
	int integer = -123;
	u32 second = 0272;
	u32 third = 0x39DE2;
	printd(integer);
	printo(second);
	printx(third);
	unsigned int var1 = 3;
	myprintf("Hello %d %s %u %c %o %x", integer, "hello", var1, 'v', 
			0610, 0x7fa);

}

void prints(char *s)
{
	int i = 0;
	for (; s[i] != '\0'; i++)
	{
		putchar(s[i]);
	}
}

#define BASE 10

char *table = "0123456789ABCDEF";

int rpu(u32 x)
{
  char c;
  if (x){
     c = table[x % BASE];
     rpu(x / BASE);
     putchar(c);
  }
} 

int printu(u32 x)
{
  if (x==0)
     putchar('0');
  else
     rpu(x);
  putchar(' ');
}

#define BASE2 10

//Print an integer (Including negitive)
int printd(int x)
{
	if (x==0)
		putchar('0');
	else
		rpu2(x);
	putchar(' ');	
}

void rpu2(int x)
{
	char c;
	if (x)
	{
		if(x < 0)
		{
			putchar('-');
			x *= -1;
		}
		c = table[x % BASE2];
		rpu2(x / BASE2);
		putchar(c);
	}

}

#define BASE3 8

//Print x in OCTal as 0...
int printo(u32 x)
{
	if (x == 0)
		putchar('0');
	else
		rpu3(x);
	putchar(' ');
}

void rpu3(u32 x)
{
	char c;
	if (x)
	{
		c = table[x % BASE3];
		rpu3(x / BASE3);
		putchar(c);
	}
}

#define BASE4 16

//Print x in HEX. as 0x...
int printx(u32 x)
{
	if (x == 0)
		putchar('0');
	else
		rpu4(x);
	putchar(' ');
}

void rpu4 (u32 x)
{
	char c;
	if (x)
	{
		c = table[x % BASE4];
		rpu4(x / BASE4);
		putchar(c);
	}
}

void myprintf(char *fmt, ...)
{
	va_list argptr;
	int i = 0;
	//Point at the format string
	char *cp;
	cp = fmt;
	//Points to the first item to be printed on stack
	//int *ip =

	va_start(argptr, fmt);

	for (; *cp != '\0'; cp++)
	{
		if (*cp != '%')
		{
			putchar(*cp);
			continue;
		}

		switch(*++cp)
		{
			case 'c':
				i = va_arg(argptr, int);
				putchar(i);
				break;
			case 'd':
				i = va_arg(argptr, int);
				printd(i);
				break;
			case 's':
				i = va_arg(argptr, int);
				prints(i);
				break;
			case 'u':
				i = va_arg(argptr, int);
				printu(i);
				break;
			case 'o':
				i = va_arg(argptr, int);
				printo(i);
				break;
			case 'x':
				i = va_arg(argptr, int);
				printx(i);
				break;
			case '%':
				putchar('%');
				break;				
		}
	}
	va_end(argptr);
}
