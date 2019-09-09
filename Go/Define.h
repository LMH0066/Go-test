#pragma once
#include<stdio.h>
#include <string.h>
class Define
{
public:
	Define();
	~Define();
	typedef struct PMove
	{
		char x;
		char y;
		double score;
	}PMove;		//�з�

	typedef struct PGroup
	{
		int count;
		double eyes;
		char color;
	}PGroup;

	typedef struct GPlayer
	{
		int total;
		int know;
	}GPlayer;

	const	static	int BOARD_ROWS = 11;		//��������������1..9Ϊ������꣬����Ϊ�߽�
	const	static	int PIECES_SUM = 81;		//�ɷ���������
	const	static	int BLACK = 1;
	const	static	int WHITE = 2;
	const	static	int BORDER = 3;				//�߽�
	const	static	int NOSTONE = 0;			//��������
	char	board[BOARD_ROWS][BOARD_ROWS];		//����
	char	board_know[BOARD_ROWS][BOARD_ROWS];	//��̽����
	static	int		boardBaseScore[BOARD_ROWS][BOARD_ROWS];//���̵Ļ�������
	char	myColor;
	char	opColor;
	PMove	bestMove;
	GPlayer player[3];						//�׺ͺ���,��¼����������������֪���Է�������
	void	BoardShow();
	void	InitGame();

	bool IsInBoard(int x, int y, char t_board[BOARD_ROWS][BOARD_ROWS] = NULL) {
		if (t_board == NULL)
		{
			t_board = board;
		}
		switch (t_board[x][y])
		{
		case BORDER:
		case NOSTONE:
			return false;
		default:
			return true;
		}
	}
	void PrintTempBoard(char m_board[][BOARD_ROWS]) {
		printf("temp_board\n  ");
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
				switch (m_board[j][i])
				{
				case 0:
					printf(" -");
					break;
				case BLACK:
					printf(" X");
					break;
				case WHITE:
					printf(" O");
					break;
				}
			}
			printf("\n");
		}
		return;
	}
};