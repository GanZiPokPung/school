#include <stdio.h>

typedef struct {
    int x, y;
} Location2D;

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
    struct _Node *node;
} Node;

typedef struct {
    Node *head;
} Stack;

Stack Stack_create()
{
    Stack stack = { NULL };
    return stack;
}

void Stack_push(Stack *stack, Element e)
{
    // modify here
}

Element Stack_pop(Stack *stack)
{
    // modify here
    return Location2D_create(0, 0);
}

void printElement(Element e)
{
    printf("(%d, %d) ", e.x, e.y);
}

void Stack_print(Stack *stack)
{
    // traverse here
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
    Stack_print(&stack);
    printElement(Stack_pop(&stack));
    Stack_print(&stack);
    printElement(Stack_pop(&stack));
    Stack_print(&stack);
}

int main(void)
{
    testStack();
    return 0;
}
