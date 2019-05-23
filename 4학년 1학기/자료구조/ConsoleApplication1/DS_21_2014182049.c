#include <stdio.h>
#include <stdlib.h>

typedef int Element;
typedef struct _BTNode
{
	Element data;
	struct _BTNode *left;
	struct _BTNode *right;
}BTNode;

/* For level traverse*/

#define MAX_QUEUE_SIZE 100
BTNode* queue[MAX_QUEUE_SIZE];
int front;
int rear;

void enqueue(BTNode* node)
{
	if (rear == MAX_QUEUE_SIZE - 1)
		return;

	rear++;
	queue[rear] = node;
}

BTNode* dequeue()
{
	BTNode *node = NULL;

	if (front == rear)
		return node;

	front++;
	node = queue[front];
	return node;
}

BTNode* BTNode_create(Element data, BTNode *left, BTNode *right)
{
	BTNode *node = (BTNode*)malloc(sizeof(BTNode));
	node->data = data;
	node->left = left;
	node->right = right;
	return node;
}

void BTNode_destroy(BTNode *node)
{
	if (node->left) BTNode_destroy(node->left);
	if (node->right) BTNode_destroy(node->right);
	free(node);
}

int BTNode_countHeight(BTNode *node)
{
	int height = 0;
	if (node != NULL)
	{
		/* 1씩 증가하며 자식 노드 둘중에서 가장 높이가 큰 높이를 가지고 온다. */
		height = 1 + max(BTNode_countHeight(node->left), BTNode_countHeight(node->right));
	}
	return height;
}

int BTNode_countLeaf(BTNode *node)
{
	int count = 0;
	if (node->left)
	{
		count += BTNode_countLeaf(node->left);
	}
	if (node->right)
	{
		count += BTNode_countLeaf(node->right);
	}
	if (count == 0)
	{
		// I am the leaf
		count = 1;
	}
	return count;
}

int BTNode_countNode(BTNode *node)
{
	int count = 0;

	if (node->left)
	{
		++count;
		count += BTNode_countNode(node->left);
	}
	if (node->right)
	{
		++count;
		count += BTNode_countNode(node->right);
	}

	return count;
}

void BTNode_traversePreOrder(BTNode *node)
{
	if (node)
	{
		printf("%d ", node->data);
		BTNode_traversePreOrder(node->left);
		BTNode_traversePreOrder(node->right);
	}
}

void BTNode_traverseInOrder(BTNode *node)
{
	if (node)
	{	
		BTNode_traverseInOrder(node->left);
		printf("%d ", node->data);
		BTNode_traverseInOrder(node->right);
	}
}

void BTNode_traversePostOrder(BTNode *node)
{
	if (node)
	{
		BTNode_traversePostOrder(node->left);
		BTNode_traversePostOrder(node->right);
		printf("%d ", node->data);
	}
}

void BTNode_traverseLevelOrder(BTNode *node)
{
	BTNode *p;

	front = -1;
	rear = -1;

	if (node == NULL)
		return;

	// root node 엔큐
	enqueue(node);

	while (1)
	{
		p = dequeue();
		if (p == NULL)
			break;

		printf("%d ", p->data);

		if (p->left != NULL)
			enqueue(p->left);

		if (p->right != NULL)
			enqueue(p->right);
	}
}

int main()
{
	BTNode *l4 = BTNode_create(4, NULL, NULL);
	BTNode *r5 = BTNode_create(5, NULL, NULL);
	BTNode *l3 = BTNode_create(3, l4, r5);

	BTNode *l7 = BTNode_create(7, NULL, NULL);
	BTNode *r8 = BTNode_create(8, NULL, NULL);
	BTNode *r6 = BTNode_create(6, l7, r8);

	BTNode *l2 = BTNode_create(2, l3, r6);

	BTNode *l10 = BTNode_create(10, NULL, NULL);
	BTNode *r11 = BTNode_create(11, NULL, NULL);
	BTNode *r9 = BTNode_create(9, l10, r11);

	BTNode *root = BTNode_create(1, l2, r9);


	printf("BTNode_countLeaf : %d\n", BTNode_countLeaf(root));
	printf("BTNode_countNode : %d\n", BTNode_countNode(root));
	printf("BTNode_countHeight : %d\n", BTNode_countHeight(root));

	printf("BTNode_traversePreOrder : ");
	BTNode_traversePreOrder(root);
	printf("\n");
	
	printf("BTNode_traverseInOrder : ");
	BTNode_traverseInOrder(root);
	printf("\n");
	
	printf("BTNode_traversePostOrder : ");
	BTNode_traversePostOrder(root);
	printf("\n");

	printf("BTNode_traverseLevelOrder : ");
	BTNode_traverseLevelOrder(root);
	printf("\n");

	BTNode_destroy(root);

	return;
}