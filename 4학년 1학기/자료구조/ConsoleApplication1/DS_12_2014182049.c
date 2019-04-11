#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int x, y;
}Location2D;

Location2D Location2D_create(int x, int y)
{
	Location2D loc;
	loc.x = x;
	loc.y = y;
	return loc;
}
typedef Location2D Element;

typedef struct _Node {
	Element data;
	struct _Node *next;
}Node;

typedef struct {
	Node *head;
}Stack;

Stack Stack_create()
{
	Stack stack = { NULL };
	return stack;
}

void error(const char* msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

void Stack_push(Stack *stack, Element e)
{
	Node *node = (Node *)malloc(sizeof(Node));
	if (node == NULL) {
		error("malloc() failed. Memory full?");
	}
	node->data = e;
	node->next = stack->head;
	stack->head = node;
}

Element Stack_pop(Stack *stack)
{
	Node* head = stack->head;
	if (head == NULL) {
		error("Stack_pop() while stack is empty");
	}
	Element e = head->data;
	stack->head = stack->head->next;
	free(head);

	return e;
}

Element *Stack_getTopRef(Stack *stack)
{
	Node* head = stack->head;
	if (head == NULL) {
		error("Stack_getTopRef() while stack is empty");
	}
	return &head->data;
}

void printElement(Element e)
{
	printf("(%d %d) ", e.x, e.y);
}

void Stack_print(Stack *stack)
{
	Node *node = stack->head;
	while (node) {
		printElement(node->data);
		node = node->next;
	}
	printf("\n");
}

void testStack()
{
	Stack stack = Stack_create();
	Stack_push(&stack, Location2D_create(1, 2));
	Stack_print(&stack);
	Stack_push(&stack, Location2D_create(2, 2));
	Stack_print(&stack);
	Stack_push(&stack, Location2D_create(3, 2));
	Stack_print(&stack);

	printElement(Stack_pop(&stack));
	printf("\n");
	Stack_print(&stack);
	printElement(Stack_pop(&stack));
	printf("\n");
	Stack_print(&stack);
	printElement(Stack_pop(&stack));
	printf("\n");
	Stack_print(&stack);

	Stack_pop(&stack);
}

int main(void)
{
	testStack();
	return 0;
}