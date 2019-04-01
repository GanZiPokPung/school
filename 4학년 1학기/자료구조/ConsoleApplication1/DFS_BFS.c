#include <stdio.h>
#include <stdlib.h>
#define MAX_QUEUE_SIZE 100

typedef struct 
{
	int x;
	int y;
}Location2D;

typedef Location2D Element;

#define MAZE_SIZE 6
char map[MAZE_SIZE][MAZE_SIZE] = {
	{'1','1','1','1','1','1' },
	{'0','0','1','0','0','1' },
	{'1','0','0','0','1','1' },
	{'1','0','1','0','1','1' },
	{'1','0','1','0','0','x' },
	{'1','1','1','1','1','1' }
};

Location2D getLocation2D(int x, int y)
{
	Location2D p;
	p.x = x;
	p.y = y;
	return p;
}

int is_valid(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAZE_SIZE || y >= MAZE_SIZE)
		return 0;

	else
		return map[y][x] == '0' || map[y][x] == 'x';
}

int DFS()
{
	int x, y;
	Location2D here;

	init_deque();
	add_rear(getLocation2D(0, 1));
	printf("DFS : ");

	while (is_empty() == 0)
	{
		here = delete_rear();
		printf("(%2d, %2d)", here.x, here.y);
		
		x = here.x;
		y = here.y;

		if (map[y][x] == 'x')
			return 1;

		else
		{
			map[y][x] = '.';
			if (is_valid(x - 1, y))
				add_rear(getLocation2D(x - 1, y));
			if (is_valid(x + 1, y))
				add_rear(getLocation2D(x + 1, y));
			if (is_valid(x, y - 1))
				add_rear(getLocation2D(x, y - 1));
			if (is_valid(x, y + 1))
				add_rear(getLocation2D(x, y + 1));
		}
	}

	return 0;
}

int BFS()
{
	int x, y;
	Location2D here;

	init_deque();
	add_rear(getLocation2D(0, 1));
	printf("BFS: ");
	while (is_empty() == 0)
	{
		here = delete_front();
		printf("(%2d, %2d)", here.x, here.y);
		x = here.x;
		y = here.y;
		if (map[y][x] == 'x')
			return 1;
		else
		{
			map[y][x] = '.';
			if (is_valid(x - 1, y))
				add_rear(getLocation2D(x - 1, y));
			if (is_valid(x + 1, y))
				add_rear(getLocation2D(x + 1, y));
			if (is_valid(x, y - 1))
				add_rear(getLocation2D(x, y - 1));
			if (is_valid(x, y + 1))
				add_rear(getLocation2D(x, y + 1));
		}
	}

	return 0;
}

int main()
{
	//printf("->%s\n", DFS() ? "성공" : "실패");
	printf("->%s\n", BFS() ? "성공" : "실패");
}