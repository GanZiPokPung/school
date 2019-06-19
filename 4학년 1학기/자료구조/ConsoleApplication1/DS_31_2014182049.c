#include <stdio.h>

#define CX_BOARD 8
#define CY_BOARD 8

// 
#define PRINTS_SOLUTION
//#define PRINTS_VERBOSE

// �ִ�
#define FULL_ORDER ( CX_BOARD * CY_BOARD )

// ü�� ���� ���� �ȿ����� �� �� �ִ����� �˻�
#define IS_VALID(x, y) ((x) >= 0 && (x) < CX_BOARD && (y) >= 0 && (y) < CY_BOARD)

typedef struct _Cell {
    int order;		// ����Ʈ�� ������� �湮?
    int way;		// ���� 0 ~ 7 ����
} Cell;

typedef struct _Move {
    int x, y;	// �̵��� �Ÿ�?
} Move;

// ���� ������ �� �ð� ���� ������ �迭�� ǥ��
Move moves[] = {
    {  2, -1 }, {  1, -2 }, { -1, -2 }, { -2, -1 },
    { -2,  1 }, { -1,  2 }, {  1,  2 }, {  2,  1 }
};

// ü�� ����
Cell board[CY_BOARD][CX_BOARD];

// ���� ���
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

// ���� ������ ���� ������ ����Ѵ�.
int countNextMoveFrom(int posX, int posY)
{
	int count = 0;
	int w, nx, ny;
	for (w = 0; w < 8; w++) {
		Move m = moves[w];
		nx = posX + m.x;
		ny = posY + m.y;

		// ���� ������ ��ġ�� ���� �ȿ� �����ϴ���
		// ���� �ѹ��� ����ġ�� ���� ��ġ���� 
		// �˻��ϰ� ���̸� ���� ����
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

	// ���� ��ġ ����
    board[y][x].order = order;

    while (loop) {
        int w, nx, ny;
		int minCount = 8;
		int minWay = -1;
        for (w = 0; w < 8; w++) {
			// �� ��ġ�� �̵��ϸ鼭
            Move m = moves[w];
            nx = x + m.x;
            ny = y + m.y;
            // printf("w=%d nx=%d ny=%d\n", w, nx, ny);

			/* ���� �ٱ����� ������ */
            if (!IS_VALID(nx, ny)) {
                // printf(" - not valid\n");
                continue;
            }

			/* ���� ��ġ�� �̹� ���� ������ ���̶�� */
			if (board[ny][nx].order != 0) {
				continue;
			}

            // printf("board[ny][nx].order=%d\n", board[ny][nx].order);

			/* �̵��� ��ġ ������ ��� �������� �̵� ���������� ���� */
			int count = countNextMoveFrom(nx, ny);

			#ifdef PRINTS_VERBOSE
			printf("w=%d cnt=%d\n", w, count);
			#endif

			if (minCount > count)
			{
				/* �ּҰ��� ã�� �� ������ ����. */
				/* �� ������ �� �� �ִ��� */

				// �ּ� ī��� 
				// �ּ� ����
				minCount = count;
				minWay = w;
			}

			/* �ѹ��� �湮���� �ʾҴٸ� */
			/*if (board[ny][nx].order == 0) {
                break;
            }*/
        }
        // printf("w=%d nx=%d ny=%d ", w, nx, ny);
        // printf("board[ny][nx].order=%d\n", board[ny][nx].order);
       
		#ifdef PRINTS_VERBOSE
		printf("MinCount=%d MinWay=%d\n", minCount, minWay);
		#endif
		
		// �� ������ �������� ������
		if (minWay < 0)
		{
			printf("Stuck Exiting\n");
			break;
		}

		// �� �������� ���ư�
		w = minWay;
		Move m = moves[w];
		nx = x + m.x;
		ny = y + m.y;

		/* �� ������ ã������ */
		if (w < 8) {
			// ���ư�
            x = nx; y = ny; order++;
            board[y][x].order = order;
            board[y][x].way = w;

			// �� Ƚ���� ü������ ũ��� ���ٸ�
			// ��� ���������̹Ƿ�
            if (order == FULL_ORDER) {
				/* ��� ���� �������� �ַ���� ã�Ҵ�! */
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

		/* ���̻� �� ������ ������ */
        if (w == 8) {
			/* ���� ��ġ�� order�� ���� */
            board[y][x].order = 0;
			/* ���� ��ġ�� �ǵ��� ����. */
            way = board[y][x].way;
            Move m = moves[way];
            #ifdef PRINTS_VERBOSE
            printf("Was (%d, %d), order=%d, way=%d\n", x, y, order, way);
            #endif
            x = x - m.x;
            y = y - m.y;
			// ���� ������ �������� ���� ��������
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