#include <stdio.h>
#include <stdlib.h>

void usingPointers()
{
	//Value is a standard variable, holding the integer 5
	int value = 5;
	
	//*Pointer creates an interger pointer which is 
	//mean to hold the address of another variable.
	//In this case, &value returns the address of value
	int *pointer = &value;

	printf("Pointer holds address of value: %p\n", (void *) pointer);
	printf("You can return the value of the pointer: %d\n", *pointer);
	printf("Getting address of value directly: %p\n",(void *) &value);
	printf("Getting address of the pointer: %p\n", (void *) &pointer);
}

void receiving (char *temp, int *temp2)
{
	printf("Received: %s\n", temp);
	printf("Received: %d\n", *temp2);
	(*temp2)++;
}

void passing()
{
	char * temp = "Hello";
	int num = 5;
	receiving(temp, &num);
	printf("Modified num variable: %d\n", num);
}

struct node {
	int data;
	struct node *next;
};

void usingStructs ()
{
	struct node *test = (struct node *) malloc( sizeof(struct node) );
	test->data = 5;
	printf("Dereferenced struct and accessing data: %d\n", test->data);
}

int main ()
{
	usingPointers ();
	passing ();
	usingStructs ();
}
