#include <stdio.h>
#include <stdlib.h>
typedef char TElement;
typedef struct BinTrNode
{
	TElement data;
	struct BinTrNode* left;
	struct BinTrNode* right;
}TNode;

TNode *root;

void init_tree() { root = NULL; }
int is_empty_tree() { return root == NULL; }
TNode* get_root() { return root; }

/* For level traverse*/
#pragma region queue
#define MAX_QUEUE_SIZE 100
#define TRUE  1
#define FALSE 0
TNode* queue[MAX_QUEUE_SIZE];
int front;
int rear;

int isEmpty()
{
	if (front == rear)
		return TRUE;

	return FALSE;
}

void enqueue(TNode* node)
{
	if (rear == MAX_QUEUE_SIZE - 1)
		return;

	rear++;
	queue[rear] = node;
}

TNode* dequeue()
{
	TNode *node = NULL;

	if (front == rear)
		return node;

	front++;
	node = queue[front];
	return node;
}
#pragma endregion

void preorder(TNode *n)
{
	if (n != NULL)
	{
		printf("[%c] ", n->data);
		preorder(n->left);
		preorder(n->right);
	}
}

void inorder(TNode *n)
{
	if (n != NULL)
	{
		preorder(n->left);
		printf("[%c] ", n->data);
		preorder(n->right);
	}
}

void postorder(TNode *n)
{
	if (n != NULL)
	{
		preorder(n->left);
		preorder(n->right);
		printf("[%c] ", n->data);
	}
}

TNode* create_tree(TElement val, TNode *l, TNode *r)
{
	TNode *n = (TNode*)malloc(sizeof(TNode));
	n->data = val;
	n->left = l;
	n->right = r;
	return n;
}


void levelorder(TNode *node)
{
	TNode *p;

	front = -1;
	rear = -1;

	if (node == NULL)
		return;

	enqueue(node);

	while (1)
	{
		p = dequeue();
		if (p == NULL)
			break;

		printf("[%c] ", p->data);

		if (p->left != NULL)
			enqueue(p->left);

		if (p->right != NULL)
			enqueue(p->right);
	}
}

int count_node(TNode *n)
{
	if (n == NULL) return 0;
	return 1 + count_node(n->left) + count_node(n->right);
}

int count_leaf(TNode *n)
{
	if (n == NULL) return 0;
	if (n->left == NULL && n->right == NULL) return 1;
	else return count_leaf(n->left) + count_leaf(n->right);
}

int calc_height(TNode *n)
{
	int hLeft, hRight;
	if (n == NULL) return 0;
	hLeft = calc_height(n->left);
	hRight = calc_height(n->right);
	return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
}

int POW(int a, int b)
{
	int i = 0;
	int result = 1;

	for (i = 0; i < b; ++i)
		result *= a;

	return result;
}

int is_complete_binary_tree()
{
	int nodeFullCheck = 0;
	int nodeCountExceptLeaf = 0;

	if (root == NULL)
		return FALSE;

	/* 1. 말단 노드를 제외한 트리가 포화 상태여야 한다. */
	nodeFullCheck = POW(2, (calc_height(root) - 1)) - 1;
	nodeCountExceptLeaf = count_node(root) - count_leaf(root);

	if (nodeCountExceptLeaf != nodeFullCheck)
		return FALSE;

	front = -1;
	rear = -1;

	enqueue(root);
	while (!isEmpty())
	{
		TNode *node = dequeue();

		/* 2. 말단 노드는 왼쪽부터 채워져야 한다. */

		if (node->left == NULL && node->right != NULL)
			return FALSE;

		if (node != NULL)
		{
			if (node->left != NULL)
				enqueue(node->left);

			if (node->right != NULL)
				enqueue(node->right);
		}
	}

	return TRUE;
}

int level(TNode *r, TNode *n, int lev)
{
	if (r == NULL)
		return 0;
	
	if (r == n)
		return lev;

	if (r->left == NULL && r->right == NULL)
		return 0;

	int left = level(r->left, n, lev + 1);
	int right = level(r->right, n, lev + 1);

	if (!left)
		return right;
	else
		return left;
}

int is_balanced()
{
	int hLeft;
	int hRight;

	if (root == NULL)
		return TRUE;

	hLeft = calc_height(root->left);
	hRight = calc_height(root->right);

	if ((hLeft - hRight) <= 1 &&
		(hLeft - hRight) >= -1)
		return TRUE;


	return FALSE;
}

int path_length()
{
	int sum = 0;

	front = -1;
	rear = -1;

	if (root != NULL)
	{
		while (!isEmpty())
		{
			enqueue(root);

			while (!isEmpty())
			{
				TNode *node = dequeue();
				sum += level(root, node, 1) - 1;

				if (node != NULL)
				{
					if (node->left != NULL)
						enqueue(node->left);

					if (node->right != NULL)
						enqueue(node->right);
				}
			}
		}
	}

	return sum;
}

void reverse(TNode *n)
{
	if (n == NULL)
		return;

	else
	{
		reverse(n->left);
		reverse(n->right);

		TNode *temp = n->left;
		n->left = n->right;
		n->right = temp;
	}
}

int main()
{
	TNode *b, *c, *d, *e, *f;

	init_tree();
	c = create_tree('C', NULL, NULL);
	d = create_tree('D', NULL, NULL);
	b = create_tree('B', c, d);
	f = create_tree('F', NULL, NULL);
	e = create_tree('E', NULL, f);

	root = create_tree('A', b, e);

	printf("\nLevel_Order : ");	levelorder(root);
	printf("\n");

	int check = is_complete_binary_tree();
	if (check == TRUE)
	{
		printf("완전 이진 트리 입니다.\n");
	}
	else
	{
		printf("완전 이진 트리가 아닙니다.\n");
	}

	printf("노드의 레벨은 %d\n", level(root, root, 1));
	printf("노드의 레벨은 %d\n", level(root, b, 1));
	printf("노드의 레벨은 %d\n", level(root, e, 1));
	printf("노드의 레벨은 %d\n", level(root, c, 1));
	printf("노드의 레벨은 %d\n", level(root, d, 1));
	printf("노드의 레벨은 %d\n", level(root, f, 1));

	check = is_balanced();
	if (check == TRUE)
	{
		printf("균형잡힌 트리입니다.\n");
	}
	else
	{
		printf("균형 잡힌 트리가 아닙니다.\n");
	}

	printf("전체 경로의 길이는 %d입니다.\n\n", path_length());


	printf("트리의 좌우를 교환합니다.");
	reverse(root);
	printf("\nLevel_Order : ");	levelorder(root);
	printf("\n");

	return 0;
}