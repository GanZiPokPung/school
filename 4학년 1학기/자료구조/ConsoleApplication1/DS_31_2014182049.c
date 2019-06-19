#include <stdio.h>

#define CX_BOARD 8
#define CY_BOARD 8

// 
#define PRINTS_SOLUTION
//#define PRINTS_VERBOSE

// 최대
#define FULL_ORDER ( CX_BOARD * CY_BOARD )

// 체스 말이 보드 안에서만 갈 수 있는지를 검사
#define IS_VALID(x, y) ((x) >= 0 && (x) < CX_BOARD && (y) >= 0 && (y) < CY_BOARD)

typedef struct _Cell {
    int order;		// 나이트가 몇번쨰로 방문?
    int way;		// 방향 0 ~ 7 까지
} Cell;

typedef struct _Move {
    int x, y;	// 이동할 거리?
} Move;

// 가는 방향을 반 시계 방향 순서로 배열로 표현
Move moves[] = {
    {  2, -1 }, {  1, -2 }, { -1, -2 }, { -2, -1 },
    { -2,  1 }, { -1,  2 }, {  1,  2 }, {  2,  1 }
};

// 체스 보드
Cell board[CY_BOARD][CX_BOARD];

// 보드 출력
void printBoard(char *title, int number)
{
    printf("=============== %s %3d\n", title, number);
    for (int y = 0; y < CY_BOARD; y++) {
        for (int x = 0; x < CX_BOARD; x++) {
            printf("%3d", board[y][x].order);
        }
        printf("\n");
    }
}

// 다음 움직일 곳의 개수를 계산한다.
int countNextMoveFrom(int posX, int posY)
{
	int count = 0;
	int w, nx, ny;
	for (w = 0; w < 8; w++) {
		Move m = moves[w];
		nx = posX + m.x;
		ny = posY + m.y;

		// 다음 움직일 위치가 보드 안에 존재하는지
		// 말이 한번도 지나치지 않은 위치인지 
		// 검사하고 참이면 갯수 증가
		if (IS_VALID(nx, ny) && board[ny][nx].order == 0) {
			count++;
		}
	}

	return count;
}

int main(void)
{
    int x = 0, y = 0;
    int order = 1;
    int way = 0;
    int loop = 1;
    int solution = 0;

	// 시작 위치 지정
    board[y][x].order = order;

    while (loop) {
        int w, nx, ny;
		int minCount = 8;
		int minWay = -1;
        for (w = 0; w < 8; w++) {
			// 각 위치를 이동하면서
            Move m = moves[w];
            nx = x + m.x;
            ny = y + m.y;
            // printf("w=%d nx=%d ny=%d\n", w, nx, ny);

			/* 보드 바깥으로 나가면 */
            if (!IS_VALID(nx, ny)) {
                // printf(" - not valid\n");
                continue;
            }

			/* 가는 위치가 이미 말이 지나간 곳이라면 */
			if (board[ny][nx].order != 0) {
				continue;
			}

            // printf("board[ny][nx].order=%d\n", board[ny][nx].order);

			/* 이동할 위치 다음에 어느 방향으로 이동 가능한지의 개수 */
			int count = countNextMoveFrom(nx, ny);

			#ifdef PRINTS_VERBOSE
			printf("w=%d cnt=%d\n", w, count);
			#endif

			if (minCount > count)
			{
				/* 최소값을 찾아 그 곳으로 가자. */
				/* 몇 군데로 갈 수 있는지 */

				// 최소 카운드 
				// 최소 방향
				minCount = count;
				minWay = w;
			}

			/* 한번도 방문하지 않았다면 */
			/*if (board[ny][nx].order == 0) {
                break;
            }*/
        }
        // printf("w=%d nx=%d ny=%d ", w, nx, ny);
        // printf("board[ny][nx].order=%d\n", board[ny][nx].order);
       
		#ifdef PRINTS_VERBOSE
		printf("MinCount=%d MinWay=%d\n", minCount, minWay);
		#endif
		
		// 갈 방향이 존재하지 않으면
		if (minWay < 0)
		{
			printf("Stuck Exiting\n");
			break;
		}

		// 갈 방향으로 나아감
		w = minWay;
		Move m = moves[w];
		nx = x + m.x;
		ny = y + m.y;

		/* 갈 방향을 찾았으면 */
		if (w < 8) {
			// 나아감
            x = nx; y = ny; order++;
            board[y][x].order = order;
            board[y][x].way = w;

			// 간 횟수가 체스판의 크기와 같다면
			// 모두 지나간것이므로
            if (order == FULL_ORDER) {
				/* 모든 곳을 지나가는 솔루션을 찾았다! */
                #ifdef PRINTS_VERBOSE
                printf("=== Found !!! ===\n");
                #endif
                solution++;
                #ifdef PRINTS_SOLUTION
                printBoard("solution", solution);
                #endif
                w = 8;
				break;
            }
            way = 0;
        } 

		/* 더이상 갈 방향이 없으면 */
        if (w == 8) {
			/* 현재 위치는 order를 비우고 */
            board[y][x].order = 0;
			/* 이전 위치로 되돌아 간다. */
            way = board[y][x].way;
            Move m = moves[way];
            #ifdef PRINTS_VERBOSE
            printf("Was (%d, %d), order=%d, way=%d\n", x, y, order, way);
            #endif
            x = x - m.x;
            y = y - m.y;
			// 갔던 방향이 막혔으니 다음 방향으로
            way++;
            order--;
            #ifdef PRINTS_VERBOSE
            printf("Now back to (%d, %d), order=%d, way=%d\n", x, y, order, way);
            #endif
            if (order == 0) {
                loop = 0;
            }
        }

        // printf("=============== %3d\n", order);
        #ifdef PRINTS_VERBOSE
        printBoard("order", order);
        #endif
    }
    return 0;
}