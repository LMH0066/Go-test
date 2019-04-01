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
	int		boardGroup[BOARD_ROWS][BOARD_ROWS] = { 0 };	//��¼��Ӧ�����ڵڼ���Ⱥ����
	PGroup	group[PIECES_SUM];							//Ⱥ�ڵ���Ϣ
	bool	GetGoMove(PMove* pMove);					//��ͨ�Ĳ����з�����pass������false�����򷵻�true�Լ��з�
	int		CountGroup(int x, int y, int index, int dir = 0);
	bool	SearchMove(PMove* pMove);					//��pass������false�����򷵻�true�Լ��з�
	virtual void	Move();
	virtual void	MoveSucceed();
	virtual void	MoveFail();
	virtual bool	nbMove(PMove* pMove) = 0;			//�߼������з�
	virtual double	GetScore(int x, int y);
	virtual int     clean(PMove pMove, char color, char temp_board[][BOARD_ROWS]) = 0;
};

