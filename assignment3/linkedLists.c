#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *next;
};

void insertInOrder (int num, struct node *root)
{
	struct node temp;
	root = (struct node *) malloc( sizeof(struct node) );		
}

void insertBeginning (int num, struct node *root)
{
	struct node *new, *temp;
	new = (struct node *) malloc( sizeof(struct node) );		
	temp = root;
	root->next = new;
	new->next = temp;
}

void insertEnd ()
{

}

int deleteNode (int num)
{

}

int getLength ()
{
	
}

void printList (struct node *root)
{
	while(root->next != '\0')
	{
		printf("%d -> ", root->data);
		root = root->next;
	}
	printf("\'\\0\'");
}

int getNthNode (int n)
{

}

int main ()
{
	struct node *root;
	insertBeginning (5, root);
}
	
