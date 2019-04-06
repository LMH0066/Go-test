#pragma once
#include "CEngine.h"
class CMonteCarlo :
	public CEngine
{
public:
	CMonteCarlo();
	~CMonteCarlo();
	const	static	int MT_NUMS = 1000;			//蒙特卡洛次数
	const	static	int TRUST_VALUE = 2000;		//信任值
	char	temp_board[BOARD_ROWS][BOARD_ROWS];
	char	visited[BOARD_ROWS][BOARD_ROWS];
	int		stone_num[4], eye[4];
	void	boardCopy();
	bool	isEye(PMove pMove, char temp_board[][BOARD_ROWS]);
	void	createEye(PMove pMove, char color, char temp_board[][BOARD_ROWS]);
	void	haveEye(char t_board[BOARD_ROWS][BOARD_ROWS] = NULL);
	bool	live(PMove pMove, char color, char temp_board[][BOARD_ROWS]);
	bool	check(PMove pMove, char color, char t_board[BOARD_ROWS][BOARD_ROWS] = NULL);
	int		removed(PMove pMove, char color, char temp_board[][BOARD_ROWS]);
	int		MonteCarloMove(char color);
	virtual int     clean(PMove pMove, char color, char temp_board[][BOARD_ROWS]);

	// 通过 CEngine 继承
	virtual bool nbMove(PMove * pMove) override;
};

