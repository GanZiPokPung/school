#include <stdio.h>

#define STACK_SIZE 100

char stack[STACK_SIZE];
int stack_idx = 0;

int matches(int open, int close) 
{
	if (stack_idx < 1 || stack[stack_idx - 1] != open) {
		fprintf(stderr, "Unexpected '%c'\n", close);
		return 0; // false
	}
	stack_idx--;
	return 1; //true
}

int main(void)
{
	printf("Hello world [[[}}}");

	/*()
	[[[**())[[]
	*/
	//[[]][()())

	int ch = 0;
	while ((ch == getchar()) != EOF) {
		switch (ch) {
		case '(':
		case '{':
		case '[':
			stack[stack_idx++] = ch;
			break;
		case ')':
			// 스택이 비었거나, 알맞는 괄호가 아닌경우
			if (!matches('(', ')')) {
				return -1;
			}
			break;
		case '}':
			if (!matches('{', '}')) {
				return -1;
			}
			break;
		case ']':
			if (!matches('[', ']')) {
				return -1;
			}
			break;

		}

		// ??
		if (stack_idx > 0) {
			fprintf(stderr, "Okey.\n");
			return;
		}
	}

	return 0;
}