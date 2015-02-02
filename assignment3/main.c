#include<stdio.h>
#

#define N 100

typedef struct node{
  struct node *next;
  char name[64];
  int  id;
  int priority;
}NODE;

NODE node[N], *freeList, *readyQueue;

int init()
{
	strcpy(node[0].name, "node");
	strcpy(node[0].name, 0);
	node[0].id = 0;
	node[0].priority = 0;
	*freeList = node[0];
	NODE *temp = *freeList;
	
	//(1). each node[i]'s name=nodei; id = i; priority = 0;
	for (int i = 1; i < N; i++)
	{
		strcpy(node[i].name, "node");
		strcpy(node[i].name, i);
		node[i].id = i;
		node[i].priority = 0;
		
		*temp = temp->next; 
	}
	//(2). all N nodes are in the freeList;    
	//(3). readyQueue = 0;
}

//Enters the node pointed by p into queue BY PRIORITY
int enqueue(NODE **queue, NODE *p)
{
	NODE *temp = queue;
	while (temp->next != '\0' || p->priority <= temp->next.priority)
	{
		temp = temp->next;
	}

	Node *temp2 = temp->next;
	temp->next = *p;
	p->next = temp2;
}

//Removes the node pointed with the HIGHEST priority and returns
//its pointer
PROC *dequeue(NODE **queue)
{
	Node *temp = queue;
	Node *highest = queue;
	int hPriority = 0;

	//Find the node
	while (temp != '\0')
	{
		if (temp->priority > hpriority)
		{
			highest = temp;
			hPriority = highest->priority;
		}
		temp = temp->next;
	}

	//Remove the Node
	Node *temp2 = highest->next;
	//I need to get the node before and after the highest node

}

//Prints the node contents of the queue
int printQueue(NODE *queue)
{
	Node *temp = queue;
	while (temp != '\0')
	{
		printf("Name: %s\n Id: %d\n Priority: %d\n", 
			temp->name, temp->id, temp->priority)
		temp = temp->next;
	}
}

//Changes the priority of a node with name in queue AND reorder
//the queue if necessary
int changePriority(NODE **queue, char *name, int priority)
{
	Node *temp = queue;
	//Comparing Arrays???? --------
	while (temp != '\0' || temp->name == name)
	{
		
	}
}

void main()
{
  while(1){
    ask for a command = add |delete |chPriority 

    add: get a free node; 
         node's priority = rand() % 7; 
         enter node into readyQueue;
         print readyQueue BEFORE and AFTER add;
 
   delete: NODE *p = dequene(&readyQueue);
           enter p into freeList;
           print readyQueue BEFORE and AFTER delete;

   chPriority NAME: newPriority = rand() % 7;
                    call YOUR changePriority() to change the priority of
                    the node with NAME to newPriority;
                    as usual, print readyQueue both BEFORE and AFTER;
   }
}
