#pragma once
#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //linux
#include <unistdio.h>

class Define
{
public:
	Define();
	~Define();
	typedef struct PMove
	{
		char x;
		char y;
		int	score;
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
	GPlayer player[2];						//�׺ͺ���,��¼����������������֪���Է�������
	void	BoardShow();
	void	InitGame();
};