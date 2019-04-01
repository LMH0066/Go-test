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
	}PMove;		//招法

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

	const	static	int BOARD_ROWS = 11;		//棋盘行数，其中1..9为棋的坐标，其他为边界
	const	static	int PIECES_SUM = 81;		//可放棋子总数
	const	static	int BLACK = 1;
	const	static	int WHITE = 2;
	const	static	int BORDER = 3;				//边界
	const	static	int NOSTONE = 0;			//空子无棋
	char	board[BOARD_ROWS][BOARD_ROWS];		//棋盘
	char	board_know[BOARD_ROWS][BOARD_ROWS];	//试探棋盘
	static	int		boardBaseScore[BOARD_ROWS][BOARD_ROWS];//棋盘的基础分数
	char	myColor;
	char	opColor;
	PMove	bestMove;
	GPlayer player[2];						//白和黑棋,记录各方的走子总数和知道对方的子数
	void	BoardShow();
	void	InitGame();
};