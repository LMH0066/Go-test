#include "Define.h"



Define::Define()
{
}

Define::~Define()
{
}

int Define::boardBaseScore[BOARD_ROWS][BOARD_ROWS] = 
{
	0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,0,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,2,3,3,3,3,3,2,1,0,
	0,1,2,3,4,4,4,3,2,1,0,
	0,1,2,3,4,5,4,3,2,1,0,
	0,1,2,3,4,4,4,3,2,1,0,
	0,1,2,3,3,3,3,3,2,1,0,
	0,1,2,2,2,2,2,2,2,1,0,
	0,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0
};
void Define::BoardShow() {
	printf("  ");
	for (int j = 1; j < 10; j++)
	{
		printf("%2d", j);
	}
	printf("\n");

	for (int i = 1; i < 10; i++)
	{
		printf("%2d", i);
		for (int j = 1; j < 10; j++)
		{
			switch (board[i][j])
			{
			case 0:
				printf(" -");
				break;
			case 1:
				printf(" X");
				break;
			case 2:
				printf(" O");
				break;
			}
		}
		printf("\n");
	}
	return;
}

void Define::InitGame()
{
	memset(player, 0, sizeof(player));
	memset(board, 0, sizeof(board));
	memset(board_know, 0, sizeof(board_know));
	memset(&bestMove, 0, sizeof(bestMove));

	for (int i = 0; i < 11; i++)
	{
		board[i][0] = BORDER;
		board[0][i] = BORDER;
		board[i][10] = BORDER;
		board[10][i] = BORDER;
	}
};