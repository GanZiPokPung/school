#include <stdio.h>
#include <stdlib.h>

int get_size(char str[])
{
	int count = 0;

	while (str[count] != '\0')
	{
		++count;
	}

	return count;
}

int get_last_index(char str[])
{
	return get_size(str) - 1;
}

void reverse_contents(char dst[], char src[], int count, int max_count)
{
	if (count < 0)
	{
		dst[max_count + 1] = '\0';
		return;
	}

	dst[max_count - count] = src[count];

	return reverse_contents(dst, src, count - 1, max_count);
}

char* reverse(char str[])
{
	char * result = NULL;

	int size = get_size(str);
	int last_index = get_last_index(str);

	result = (char*)malloc(sizeof(char) * size);

	reverse_contents(result, str, last_index, last_index);

	return result;
}

int main()
{
	char *result = NULL;

	result = reverse("¾È³çÇÏ¼¼¿ä");

	printf("%s\n", result);

	return 0;
}