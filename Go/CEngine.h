#pragma once
#include "Define.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>

extern std::ofstream fout;

class CEngine :
	public Define
{
public:
	CEngine();
	~CEngine();
	int		boardGroup[BOARD_ROWS][BOARD_ROWS] = { 0 };	//记录相应的子在第几个群里面
	PGroup	group[PIECES_SUM];							//群内的信息
	bool	GetGoMove(PMove* pMove);					//普通的产生招法。若pass，返回false，否则返回true以及招法
	int		CountGroup(int x, int y, int index, int dir = 0);
	bool	SearchMove(PMove* pMove);					//若pass，返回false，否则返回true以及招法
	virtual void	Move();
	virtual void	MoveSucceed();
	virtual void	MoveFail();
	virtual bool	nbMove(PMove* pMove) = 0;			//高级生成招法
	virtual double	GetScore(int x, int y);
	virtual int     clean(PMove pMove, char color, char temp_board[][BOARD_ROWS]) = 0;
};

