#include <stdio.h>
#include <stdlib.h>

#define MAP_SIZE 16

char map[][MAP_SIZE + 1] = {
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
    "********",
    "e *    *",
    "*   ** *",
    "* * ** *",
    "**** * *",
    "*      *",
    "* ** * *",
    "*x******"
};

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
Element deque_data[MAX_DEQUE_SIZE];
int deque_front, deque_rear;
#define DEQUE_INDEX(idx, amount) \
    (((idx) + (amount) + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE)

void initDeque()
{
    deque_front = deque_rear = 0;
}

int isEmpty() {
    return deque_front == deque_rear; 
}

int isFull() {
    return DEQUE_INDEX(deque_rear, 1) == deque_front;
}

int getSize() {
    return (deque_rear - deque_front + MAX_DEQUE_SIZE) % MAX_DEQUE_SIZE;
}

void addFront(Element e)
{
    if (isFull()) {
        error("Deque full on addFront()");
    }

    deque_data[deque_front] = e;
    deque_front = DEQUE_INDEX(deque_front, -1);
}

void addRear(Element e)
{
    if (isFull()) {
        error("Deque full on addRear()");
    }
    deque_rear = DEQUE_INDEX(deque_rear, 1);
    deque_data[deque_rear] = e;
}

Element deleteFront()
{
    if (isEmpty()) {
        error("Deque empty on deleteFront()");
    }
    deque_front = DEQUE_INDEX(deque_front, 1);
    return deque_data[deque_front];
}

Element deleteRear()
{
    if (isEmpty()) {
        error("Deque empty on deleteRear()");
    }
    Element r = deque_data[deque_rear];
    deque_rear = DEQUE_INDEX(deque_rear, -1);
    return r;
}

Element getFront()
{
    if (isEmpty()) {
        error("Deque empty on getFront()");
    }
    int front = DEQUE_INDEX(deque_front, 1);
    Element r = deque_data[front];
    return r;
}

Element getRear()
{
    if (isEmpty()) {
        error("Deque empty on getRear()");
    }
    Element r = deque_data[deque_rear];
    return r;
}

void printElement(Element e)
{
    printf("(%d,%d) ", e.x, e.y);
}

void printDeque()
{
    printf("[%d-%d, %d] ", deque_front, deque_rear, getSize());
    int index = deque_front;
    while (index != deque_rear) {
        index = DEQUE_INDEX(index, 1);
        printf("<%d>", index);
        printElement(deque_data[index]);
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

void printMap()
{
    for (int y = 0; y < MAP_SIZE; y++) {
        printf("%s\n", map[y]);
    }
    printf("\n");
}

int DFS()
{
    initDeque();
    addRear(findStartPosition());
    // addRear(createLocation2D(0, 1));
    while (!isEmpty()) {
        printDeque();
        Location2D loc = deleteRear();
        // printElement(loc);
        // printf("\n");
        int x = loc.x, y = loc.y;
        if (map[y][x] == 'x') return 1;
        map[y][x] = '-';
        if (isValid(x, y - 1)) addRear(createLocation2D(x, y - 1));
        if (isValid(x, y + 1)) addRear(createLocation2D(x, y + 1));
        if (isValid(x - 1, y)) addRear(createLocation2D(x - 1, y));
        if (isValid(x + 1, y)) addRear(createLocation2D(x + 1, y));
        printMap();
    }

    return 0;
}

int BFS()
{
    initDeque();
    addRear(findStartPosition());
    // addRear(createLocation2D(0, 1));
    while (!isEmpty()) {
        printDeque();
        Location2D loc = deleteFront();
        // printElement(loc);
        // printf("\n");
        int x = loc.x, y = loc.y;
        if (map[y][x] == 'x') return 1;
        map[y][x] = '-';
        if (isValid(x, y - 1)) addRear(createLocation2D(x, y - 1));
        if (isValid(x, y + 1)) addRear(createLocation2D(x, y + 1));
        if (isValid(x - 1, y)) addRear(createLocation2D(x - 1, y));
        if (isValid(x + 1, y)) addRear(createLocation2D(x + 1, y));
        printMap();
    }

    return 0;
}

// [0 ..< 5]
// (0, 5]

void testDeque(void)
{
    initDeque();
    printDeque();
    printf("\n");
    for (int i = 1; i <= 9; ++i)
    {
        if (i % 2) {
            addFront(createLocation2D(i, 5));
        } else {
            addRear(createLocation2D(10, i));
        }
        printDeque();
        printf("\n");
    }
    printDeque();
    printf("\n");
    printElement(deleteFront());
    printElement(deleteRear());
    printElement(deleteFront());
    printf("\n");
    printDeque();
    printf("\n");
}

int main(void)
{
    // testDeque();
    DFS();
    // BFS();
    return 0;
}

















