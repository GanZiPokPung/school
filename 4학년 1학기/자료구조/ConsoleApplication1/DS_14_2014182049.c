#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VISITED '@'
#define DEADEND 'O'
// #define MAX_MAP_SIZE 1000
#define MAP_SIZE 1000

int colored = 0;
int line = 0;
char map[MAP_SIZE][MAP_SIZE + 1];

void readMap()
{
    while (fgets(map[line], MAP_SIZE, stdin)) {
        map[line][strlen(map[line]) - 1] = '\0';
        line++;
    }
}
    // "****************",
    // "e         *    *",
    // "******* *** ** *",
    // "**    *     ** *",
    // "*  ** *** ***  *",
    // "* *       * * **",
    // "* ****** **    *",
    // "* ** *** *******",
    // "* *    **      *",
    // "* ** *    ** ***",
    // "* ** ***** * * *",
    // "* *    *       *",
    // "* ** ***** *****",
    // "*      ***     *",
    // "* *****    ** **",
    // "*************x**",
    // "********",
    // "e *    *",
    // "*   ** *",
    // "* * ** *",
    // "**** * *",
    // "*      *",
    // "* ** * *",
    // "*x******"
// };

typedef struct {
    int x, y;
    int way;
} Location2D;

typedef Location2D Element;

Location2D createLocation2D(int x, int y)
{
    Location2D loc;
    loc.x = x, loc.y = y;
    loc.way = 0;
    return loc;
}

int isValid(int x, int y)
{
    // printf("isValid: %d, %d:\n", x, y);
    if (x < 0 || x >= MAP_SIZE) return 0;
    if (y < 0 || y >= MAP_SIZE) return 0;
    char ch = map[y][x];
    // printf("%c valid: %d\n", ch, ((ch == ' ' || ch == 'x') ? 1 : 0));
    return (ch == ' ' || ch == 'x') ? 1 : 0;
}

void error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

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
	printf("(%d,%d-%d) ", e.x, e.y, e.way);
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

int isEmpty(Stack *stack)
{
	if (stack->head == NULL)
	{
		error("malloc() failed. Memory full?");

		return 1;
	}
	else
	{
		return 0;
	}
}

Location2D findStartPosition()
{
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (map[y][x] == 'e') {
                return createLocation2D(x, y);
            }
        }
    }
    error("Cannot find start position");
    return createLocation2D(0, 0); // not excuted
}

void printMap()
{
    int prevColor = 0;
    for (int y = 0; y < MAP_SIZE; y++) {
        if (!map[y][0]) {
            break;
        }
        // printf("%s\n", map[y]);
        for (int x = 0; map[y][x]; x++) {
            char ch = map[y][x];
            if (!colored) {
                putchar(ch);
                continue;
            }
            int color = ch == VISITED ? 32 : ch == DEADEND ? 31 : 0;
            if (color == prevColor) {
                putchar(ch);
                continue;
            }
            prevColor = color;
            if (color) {
                printf("\x1b[%d;1m%c", color, ch);
            } else {
                printf("\x1b[0m%c", ch);
            }
        }
        if (prevColor) {
            printf("\x1b[0m");
            prevColor = 0;
        }
        putchar('\n');
    }
    printf("\n");
}

int doBackTrack()
{
	Stack stack;

	stack = Stack_create();
	
	Stack_push(&stack, findStartPosition());

    // addRear(findStartPosition());
    // addRear(createLocation2D(0, 1));
    int t = 0;
    while (!isEmpty(&stack)) {
        // if (line < 100) {
            // printDeque();
            // printMap();
        // }
        Location2D *p = Stack_getTopRef(&stack);
        printf("try %d: ", ++t);
        printElement(*p);
        printf("\n");
        int x = p->x, y = p->y;
        if (map[y][x] == 'x') {
            printMap();
            printf("Found: ");
            printElement(*p);
            return 1;
        }
        if (isValid(x, y)) {
            map[y][x] = VISITED;
        }
        //int size = getSize();
        if (p->way < 1) {
            if (isValid(x, y - 1)) {
                Stack_push(&stack ,createLocation2D(x, y - 1));
                continue;
            }
            p->way++;
        }
        if (p->way < 2) {
            if (isValid(x, y + 1)) {
                 Stack_push(&stack ,createLocation2D(x, y + 1));
                continue;
            }
            p->way++;
        }
        if (p->way < 3) {
            if (isValid(x - 1, y)) {
				Stack_push(&stack, createLocation2D(x - 1, y));
                continue;
            }
            p->way++;
        }
        if (p->way < 4) {
            if (isValid(x + 1, y)) {
				Stack_push(&stack, createLocation2D(x + 1, y));
                continue;
            }
            p->way++;
        }

        map[y][x] = DEADEND;
        Stack_pop(&stack);

        // if (line < 100) {
        //     printMap();
        // }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        colored = 1;
    }
    readMap();
    // printMap();
    // testDeque();
    doBackTrack();
    // BFS();
    return 0;
}

















