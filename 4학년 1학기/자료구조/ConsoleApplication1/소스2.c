#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//typedef struct _LineData
//{
//	char *string;
//}LineData;

typedef struct _DLNode
{
	//LineData data;
	char *string;
	struct _DLNode *prev;
	struct _DLNode *next;
}DLNode;

typedef struct _DLList
{
	DLNode *head;
	DLNode *tail;
} DLList;

DLList g_lineList;

DLList DLList_Create()
{
	DLList dll = { NULL, NULL };
	return dll;
}

void DLList_Add(DLList* dll, char *line)
{
	DLNode *node = (DLNode*)malloc(sizeof(DLNode));	
	node->string = (char*)malloc(strlen(line) + 1);
	strcpy(node->string, line);

	node->next = NULL;

	if (dll->tail)
	{
		dll->tail->next = node;
		node->prev = dll->tail;
	}
	else
	{
		dll->head = node;
		node->prev = NULL;
	}

	dll->tail = node;
}

void DLList_Print(DLList *dll)
{
	DLNode *node = dll->head;
	while (node)
	{
		//printf("%s\n",node->string);
		puts(node->string);
		node = node->next;
	}
}

int selectMenu()
{
	printf("i : Insert, d : Delete, q : Quit : ");
	char command[256];
	if (!fgets(command, sizeof(command), stdin)) 
	{
		exit(1);
	};
	// Trim
	char *p = command + strlen(command) - 1;
	while (isspace(*p))
	{
		*p-- = '\0';
	}

	if (strlen(command) != 2)
	{
		return 0;
	}
	return command[0];
}

void appendLine()
{
	char buffer[2048];
	if (!fgets(buffer, sizeof(buffer), stdin))
	{
		exit(1);
	}

	DLList_Add(&g_lineList, buffer);
}

int main()
{
	g_lineList = DLList_Create();

	while (1)
	{
		int menu = selectMenu();
		switch (menu)
		{
		case 'i':
			//insertLine();
			
			continue;
		case 'a':
			appendLine();
			continue;
		case 'p':
			DLList_Print(&g_lineList);
			continue;
		case 'd':
			continue;
		case 'q':
			// Á¾·á
			return 0;
		}
	}

	return 0;
}