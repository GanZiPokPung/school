#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 1000
//#define MAP_SIZE 51
//#define MAP_SIZE 399
//#define MAP_SIZE 997

char map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };


typedef struct {
    int x, y;
} Location2D;

typedef Location2D Element;

Location2D createLocation2D(int x, int y)
{
    Location2D loc;
    loc.x = x, loc.y = y;
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

#define MAX_DEQUE_SIZE 100
typedef struct _tagDeque {
	Element deque_data[MAX_DEQUE_SIZE];
	int deque_front, deque_rear;
}DEQUE;
//Element deque_data[MAX_DEQUE_SIZE];
//int deque_front, deque_rear;
#define DEQUE_INDEX(idx, amount) \
    (((idx) + (amount) + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE)

void initDeque(DEQUE *deque)
{
	deque->deque_front = deque->deque_rear = 0;
}

int isEmpty(DEQUE *deque) {
    return deque->deque_front == deque->deque_rear;
}

int isFull(DEQUE *deque) {
    return DEQUE_INDEX(deque->deque_rear, 1) == deque->deque_front;
}

int getSize(DEQUE *deque) {
    return (deque->deque_rear - deque->deque_front + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE;
}

void addFront(Element e, DEQUE *deque)
{
    if (isFull(deque)) {
        error("Deque full on addFront()");
    }

	deque->deque_data[deque->deque_front] = e;
	deque->deque_front = DEQUE_INDEX(deque->deque_front, -1);
}

void addRear(Element e, DEQUE *deque)
{
    if (isFull(deque)) {
        error("Deque full on addRear()");
    }
    
	deque->deque_rear = DEQUE_INDEX(deque->deque_rear, 1);
	deque->deque_data[deque->deque_rear] = e;
}

Element deleteFront(DEQUE *deque)
{
    if (isEmpty(deque)) {
        error("Deque empty on deleteFront()");
    }
	deque->deque_front = DEQUE_INDEX(deque->deque_front, 1);
    return deque->deque_data[deque->deque_front];
}

Element deleteRear(DEQUE *deque)
{
    if (isEmpty(deque)) {
        error("Deque empty on deleteRear()");
    }
    Element r = deque->deque_data[deque->deque_rear];
	deque->deque_rear = DEQUE_INDEX(deque->deque_rear, -1);
    return r;
}

Element getFront(DEQUE *deque)
{
    if (isEmpty(deque)) {
        error("Deque empty on getFront()");
    }
    int front = DEQUE_INDEX(deque->deque_front, 1);
    Element r = deque->deque_data[front];
    return r;
}

Element getRear(DEQUE *deque)
{
    if (isEmpty(deque)) {
        error("Deque empty on getRear()");
    }
    Element r = deque->deque_data[deque->deque_rear];
    return r;
}

void printElement(Element e)
{
    printf("(%d,%d) ", e.x, e.y);
}

void printDeque(DEQUE *deque)
{
    printf("[%d-%d, %d] ", deque->deque_front, deque->deque_rear, getSize(deque));
    int index = deque->deque_front;
    while (index != deque->deque_rear) {
        index = DEQUE_INDEX(index, 1);
        printf("<%d>", index);
        printElement(deque->deque_data[index]);
    }
    printf("\n");
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

void printMap(int line)
{
	if (line > 100)
		return;

    for (int y = 0; y < line; y++) {
        printf("%s\n", map[y]);
    }
    printf("\n");
}

int readFile()
{
	int line = 0;
	int i = 0;

	printf("Input your map\n\nMap? ");


	while (fgets(map[line], 997 + 1, stdin)) {
		map[line][strlen(map[line]) + 1] = '\0';
		line++;
	}

	return line;
}

int DFS()
{
	DEQUE test;
	int line;

	line = readFile();

    initDeque(&test);
    addRear(findStartPosition(), &test);
    // addRear(createLocation2D(0, 1));
    while (!isEmpty(&test)) {
        printDeque(&test);
        Location2D loc = deleteRear(&test);
        // printElement(loc);
        // printf("\n");
        int x = loc.x, y = loc.y;
        if (map[y][x] == 'x') return 1;
        map[y][x] = '.';
        if (isValid(x, y - 1)) addRear(createLocation2D(x, y - 1), &test);
        if (isValid(x, y + 1)) addRear(createLocation2D(x, y + 1), &test);
        if (isValid(x - 1, y)) addRear(createLocation2D(x - 1, y), &test);
        if (isValid(x + 1, y)) addRear(createLocation2D(x + 1, y), &test);
        printMap(line);
    }

    return 0;
}

int BFS()
{
	DEQUE test;
	int line;

	line = readFile();

    initDeque(&test);
    addRear(findStartPosition(), &test);
    // addRear(createLocation2D(0, 1));
    while (!isEmpty(&test)) {
        printDeque(&test);
        Location2D loc = deleteFront(&test);
        // printElement(loc);
        // printf("\n");
        int x = loc.x, y = loc.y;
        if (map[y][x] == 'x') return 1;
        map[y][x] = '.';
        if (isValid(x, y - 1)) addRear(createLocation2D(x, y - 1), &test);
        if (isValid(x, y + 1)) addRear(createLocation2D(x, y + 1), &test);
        if (isValid(x - 1, y)) addRear(createLocation2D(x - 1, y), &test);
        if (isValid(x + 1, y)) addRear(createLocation2D(x + 1, y), &test);
        printMap(line);
    }

    return 0;
}

void testDeque(void)
{
	DEQUE test;

    initDeque(&test);
    printDeque(&test);
    printf("\n");
    for (int i = 1; i <= 9; ++i)
    {
        if (i % 2) {
            addFront(createLocation2D(i, 5), &test);
        } else {
            addRear(createLocation2D(10, i), &test);
        }
        printDeque(&test);
        printf("\n");
    }
    printDeque(&test);
    printf("\n");
    printElement(deleteFront(&test));
    printElement(deleteRear(&test));
    printElement(deleteFront(&test));
    printf("\n");
    printDeque(&test);
    printf("\n");
}

int main(void)
{
    // testDeque();
    DFS();
    // BFS();
    return 0;
}

















