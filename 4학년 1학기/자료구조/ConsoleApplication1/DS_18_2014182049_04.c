#include <stdio.h>

int Ackermann(int m, int n)
{
	if (m == 0)
		return n + 1;

	if (m > 0 && n == 0)
		return Ackermann(m - 1, 1);

	if (m > 0 && n > 0)
	{
		return Ackermann(m - 1, Ackermann(m, n - 1));
	}
}

int main()
{
	int result = 0;
	int i, j = 0;

	for (i = 0; i <= 3; ++i)
	{
		for (j = 0; j <= 4; ++j)
		{
			result = Ackermann(i, j);
			printf("Ackermann(%d, %d)  :  %d\n", i, j, result);
		}
	}


	return 0;
}