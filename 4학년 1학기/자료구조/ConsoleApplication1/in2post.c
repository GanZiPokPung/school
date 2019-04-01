#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_STACK_SIZE 100
typedef char Element;
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

Element peek()
{
	if (isEmpty())
	{
		error("Stack is empty in peek()");
	}

	return data[stack_top];
}

int precedence(char op)
{
	switch (op)
	{
	case '(': case ')': return 0;
	}

	return -1;
}

int main(void)
{
	initStack();
	int ch;
	int pr;

	while ((ch = getchar()) != EOF)
	{
		if (isdigit(ch))
		{
			printf("%c ", ch);
			continue;
		}

		switch (ch)
		{
		case '(':
			break;
		case ')':
			while (1)
			{
				char e = pop();
				if (e == '(')
				{
					break;
				}
				printf("%c " , e);
			}
			break;
			
		case '+':
		case '-':
		case '*':
		case '/':
			pr = precedence(ch);
			while (!isEmpty())
			{
				char e = peek();
				if (precedence(e) < pr)
				{
					break;
				}
				printf("%c ", pop());	
			}
			push(ch);
			break;
		}
	}

	while (!isEmpty())
	{
		printf("%c ", pop());
	}
	printf("\n");
	return 0;
}