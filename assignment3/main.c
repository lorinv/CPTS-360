#include<stdio.h>
#include<stdlib.h>

#define N 100

typedef struct node{
  struct node *next;
  char name[64];
  int  id;
  int priority;
}NODE;

NODE nodes[N], *freeList, *readyQueue;

int init()
{
	int i;
	//(1). each node[i]'s name=nodei; id = i; priority = 0;
	for (i = 0; i < N; i++)
	{
		sprintf(nodes[i].name, "node%d", i);
		nodes[i].id = i;
		nodes[i].priority = 0;
	}

	//(2). all N nodes are in the freeList;
	freeList = nodes;

	//(3). readyQueue = 0;
	readyQueue = 0;
}

//Enters the node pointed by p into queue BY PRIORITY
int enqueue(NODE **queue, NODE *p)
{
	NODE *temp, *temp2;
	if (*queue == NULL)
	{
		*queue = p;
	}
	else
	{
		temp = *queue;
		while (1)
		{
			if (temp->next == NULL || temp->priority > p->priority)
			{
				if (p->priority < temp->priority)
				{
					*queue = p;
					p->next = temp;
				}
				else
				{
					temp->next = p;
				}
			return 1;
			} 
			if ((temp->next)->priority <= p->priority)
			{
				temp=temp->next;
			}
			else
			{
				temp2 = temp->next;
				p->next = temp2;
				temp->next = p;
				return 1;
			}
		}
	}
}

//Removes the node pointed with the HIGHEST priority and returns
//its pointer
NODE *dequeue(NODE **queue)
{
	NODE *temp;
	NODE *highest = queue;
	int hPriority = 0;

	if (*queue == NULL)
	{
		return;
	}

	temp = highest->next;
	*queue = temp;
	return highest;
}

//Prints the node contents of the queue
int printQueue(NODE *queue)
{
	NODE *temp = queue;
	while (temp != NULL)
	{
		printf("Name: %s\n Id: %d\n Priority: %d\n", temp->name, temp->id, temp->priority);
		temp = temp->next;
	}
}

//Changes the priority of a node with name in queue AND reorder
//the queue if necessary
int changePriority(NODE **queue, char *name, int priority)
{
	NODE *temp=queue;
	NODE *temp2=temp;
	while(temp != NULL)
	{
		if(strcmp(temp->name , name) == 0)
		{
			temp->priority = priority;
			if(current!=temp)
			{
				temp2->next=temp->next;
				enqueue(queue,temp);
			}
			else
			{
				temp=temp->next;
				enqueue(queue,temp);
			}
			return 1;
		}
		temp2=temp;
		temp=temp->next;
	}
}

void main()
{
  init();
  int choice = 1, index = 0;;
  while(1){
	// ask for a command = add |delete |chPriority 
	printf("Please Select Command:\n 1. Add\n 2. Delete\n 3. chPriority\n");
	scanf("%d", &choice);
	if (choice == 1)
	{
		NODE *freeNode = &nodes[index];
		index++;
		freeNode->priority = rand() % 7;
		enqueue(&readyQueue, freeNode);
		printQueue(readyQueue);
	}
	else if (choice == 2)
	{ 
		  NODE *p = dequeue(&readyQueue);
		  NODE *temp = freeList;
		  while (temp)
		  {
				temp = temp->next;			
		  }
		  temp = p;
		  printQueue(readyQueue);
	}
	/*	
           enter p into freeList;
           print readyQueue BEFORE and AFTER delete;

   chPriority NAME: newPriority = rand() % 7;
                    call YOUR changePriority() to change the priority of
                    the node with NAME to newPriority;
                    as usual, print readyQueue both BEFORE and AFTER;
	*/
   }
}
