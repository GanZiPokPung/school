#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_STACK_SIZE 100
typedef double Element;
Element data[MAX_STACK_SIZE];
int stack_top;

void error(const char * msg)
{
	// 표준 에러 출력
	fprintf(stderr, "Error: %s\n", msg);
	exit(0);
}

int isFull(void)
{
	return stack_top == MAX_STACK_SIZE - 1;
}

int isEmpty()
{
	return stack_top < 0;
}

void initStack(void)
{
	stack_top = -1;
}

// returns new stack size
int push(Element e)
{
	if (isFull())
	{
		error("Stack overflow");
	}
	data[++stack_top] = e;
	return stack_top + 1;
}

Element pop()
{
	if (isEmpty())
	{
		error("Stack underflow");
	}

	return data[stack_top--];
}

int main(void)
{
	initStack();
	int ch;
	int last = 0;
	int first = 0;
	while ((ch = getchar()) != EOF)
	{
		if (isdigit(ch)) 
		{
			push(ch - '0');
			continue;
		}
		
		switch (ch)
		{
		case '+':
			push(pop() + pop());
			break;
		case '-':
			last = pop();
			first = pop();
			push(first - last);
			break;
		case '*':
			push(pop() * pop());
			break;
		case '/':
			last = pop();
			first = pop();
			push(first / last);
			break;
		}
	}
	printf("Result : %f\n", pop());
	return 0;
}