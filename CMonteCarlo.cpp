#include "CMonteCarlo.h"



CMonteCarlo::CMonteCarlo()
{
}


CMonteCarlo::~CMonteCarlo()
{
}

//初始化temp_board
void CMonteCarlo::boardCopy()
{
	int i, j;
	for (i = 0; i <= 10; i++) {
		for (j = 0; j <= 10; j++) {
			temp_board[i][j] = board[i][j];
		}
	}
}

bool CMonteCarlo::isEye(PMove pMove, char temp_board[][BOARD_ROWS]) {
	PMove temp = pMove;
	if (temp_board[temp.x][temp.y] != 0)
		return false;
	//这里判断了左右，左上，右下，上下。我觉得不太合理，应该是左上，左下，右上，右下
	//我觉得都行，能达到判断四个子颜色相同的目的
	if (temp_board[temp.x - 1][temp.y] != temp_board[temp.x + 1][temp.y] &&
		temp_board[temp.x - 1][temp.y] != BORDER &&
		temp_board[temp.x + 1][temp.y] != BORDER)
		return false;
	if (temp_board[temp.x - 1][temp.y] != temp_board[temp.x][temp.y - 1] &&
		temp_board[temp.x - 1][temp.y] != BORDER &&
		temp_board[temp.x][temp.y - 1] != BORDER)
		return false;
	if (temp_board[temp.x + 1][temp.y] != temp_board[temp.x][temp.y + 1] &&
		temp_board[temp.x + 1][temp.y] != BORDER &&
		temp_board[temp.x][temp.y + 1] != BORDER)
		return false;
	if (temp_board[temp.x][temp.y + 1] != temp_board[temp.x][temp.y - 1] &&
		temp_board[temp.x][temp.y + 1] != BORDER &&
		temp_board[temp.x][temp.y - 1] != BORDER)
		return false;
	return true;
}

void CMonteCarlo::createEye(PMove pMove, char color, char temp_board[][BOARD_ROWS]) {
	int i;
	PMove temp = pMove;
	for (i = -1; i <= 1; i += 2) {
		temp.x = pMove.x + i;
		if (isEye(temp, temp_board)) {
			eye[color]++;
		}
		temp.x = pMove.x;
		temp.y = pMove.y + i;
		if (isEye(temp, temp_board)) {
			eye[color]++;
		}
		temp.y = pMove.y;
	}
}

// TODO: 为何要在左右判断
void CMonteCarlo::haveEye()
{
	PMove temp;
	for (temp.x = 1; temp.x <= 9; temp.x++) {
		for (temp.y = 1; temp.y <= 9; temp.y++) {
			//已经判断是眼了为什么还要判断上下是否为墙？
                        //只是为了取这个眼位属于哪个颜色的棋子
			if (isEye(temp, board)) {
				if (temp_board[temp.x + 1][temp.y] != BORDER)
					eye[temp_board[temp.x + 1][temp.y]]++;
				else
					eye[temp_board[temp.x - 1][temp.y]]++;
			}
		}
	}
}

// 周边或者此处有自己的子或者为空那么返回true
bool CMonteCarlo::live(PMove pMove, char color, char temp_board[][BOARD_ROWS]) {
	int i;
	PMove temp = pMove;

	if (temp_board[pMove.x][pMove.y] == 0)
		return true;

	if (temp_board[temp.x][temp.y] != color)
		return false;

	visited[pMove.x][pMove.y] = 1;

	for (i = -1; i <= 1; i += 2) {
		temp.x = pMove.x + i;
		if (visited[temp.x][temp.y] == 0 && live(temp, color, temp_board)) {
			visited[temp.x][temp.y] = 0;
			visited[pMove.x][pMove.y] = 0;
			return true;
		}
		temp.x = pMove.x;
		temp.y = pMove.y + i;
		if (visited[temp.x][temp.y] == 0 && live(temp, color, temp_board)) {
			visited[temp.x][temp.y] = 0;
			visited[pMove.x][pMove.y] = 0;
			return true;
		}
		temp.y = pMove.y;
	}
	return false;
}

// TODO. 为什么是!live(敌方棋子)   
// 答: 感觉这里之所以要在第三个if里面 尝试性落自己的子(1), 是为了给敌方子的live判断加入条件, 假如将这个子替换为自己的子, 这时敌方无法live,那么说明这里必定是敌方子, 否则敌方周边子就不具备存在合理性
// false: 自己子, (不是自己子或落下不可存活的空点)且周边满足(有自己点或敌方可存活或是围墙)
// true : 可落空点, 敌方的连贯点
bool CMonteCarlo::check(PMove pMove, char color)
{
	char temp_color;
	PMove temp = pMove;
	int i;
	if (temp_board[pMove.x][pMove.y] == color)	//是自己的子,不能落
		return false;
	if (temp_board[pMove.x][pMove.y] == 0) {	// TODO.如果不能live岂不是证明这里是眼? 为什么不加入create眼
		temp_board[pMove.x][pMove.y] = color;
		if (live(pMove, color, temp_board))
			return true;
		temp_board[pMove.x][pMove.y] = 0;
	}
	if (board_know[pMove.x][pMove.y] < 2) {		// 进入条件: 敌方子(不是自己子)或被包围空子     且know次数小于2
		temp_color = temp_board[pMove.x][pMove.y];	// 提取该点原有子的颜色 为了的是下面的换子失败时回归
		temp_board[pMove.x][pMove.y] = color;	// 尝试性落下自己的子 (1)
		for (i = -1; i <= 1; i += 2) {			// 如果周边不是自己子 同时敌方子无法存活 同时不是边界 那么落下敌方子
			temp.x = pMove.x + i;
			if (temp_board[temp.x][temp.y] != color && !live(temp, color % 2 + 1, temp_board) && temp_board[temp.x][temp.y] != BORDER) {
				temp_board[pMove.x][pMove.y] = color % 2 + 1;
				return true;
			}
			temp.x = pMove.x;
			temp.y = pMove.y + i;
			if (temp_board[temp.x][temp.y] != color && !live(temp, color % 2 + 1, temp_board) && temp_board[temp.x][temp.y] != BORDER) {
				temp_board[pMove.x][pMove.y] = color % 2 + 1;
				return true;
			}
			temp.y = pMove.y;
		}
		temp_board[pMove.x][pMove.y] = temp_color;
	}

	return false;	
}

// remove己方连带子
int CMonteCarlo::removed(PMove pMove, char color, char temp_board[][BOARD_ROWS])
{
	int i, num = 0;
	PMove temp = pMove;
	temp_board[temp.x][temp.y] = 0;

	for (i = -1; i <= 1; i += 2) {
		temp.x = pMove.x + i;
		if (temp_board[temp.x][temp.y] == color) {
			num += removed(temp, color, temp_board) + 1;
		}
		temp.x = pMove.x;
		temp.y = pMove.y + i;
		if (temp_board[temp.x][temp.y] == color) {
			num += removed(temp, color, temp_board) + 1;
		}
		temp.y = pMove.y;
	}
	return num;
}


// 清楚己方不再存活的子 但是保留当前下的子 这是为了什么
int CMonteCarlo::clean(PMove pMove, char color, char temp_board[][BOARD_ROWS])
{
	int i, num = 0;
	PMove temp = pMove;

	for (i = -1; i <= 1; i += 2) {
		temp.x = pMove.x + i;
		if (temp_board[temp.x][temp.y] == color && !live(temp, color, temp_board)) {
			num += removed(temp, color, temp_board) + 1;
		}
		temp.x = pMove.x;
		temp.y = pMove.y + i;
		if (temp_board[temp.x][temp.y] == color && !live(temp, color, temp_board)) {
			num += removed(temp, color, temp_board) + 1;
		}
		temp.y = pMove.y;

	}
	return num;
}

// 本质操作是在敌我双方在随机数下轮流进行落子, 但存在一个问题:随机性.这样子就没有利用好既有的落子结构.
int CMonteCarlo::MonteCarloMove(char color)
{
	int num, flag, take_num, out = 0;
	PMove temp;
	memset(eye, 0, sizeof(eye));
	haveEye();
	while (1) {
		if (stone_num[1] + stone_num[2] + eye[1] + eye[2] >= 81)
			break;

		//PrintTempBoard();
		srand(rand()*(int)time(0));
		num = 1 + (int)((81.0 - stone_num[1] - stone_num[2] - eye[1] - eye[2])*rand() / (RAND_MAX + 1.0));
		flag = 0;
		for (temp.x = 1; temp.x <= 9; temp.x++) {
			for (temp.y = 1; temp.y <= 9; temp.y++) {
				if (temp_board[temp.x][temp.y] == 0) {
					num--;
					if (num <= 1) {
						//printf("1\n");
						if (check(temp, color)) {
							flag = 1;
							//printf("2\n");
							temp_board[temp.x][temp.y] = color;
							stone_num[color] ++;
							//PrintTempBoard();
							take_num = clean(temp, color % 2 + 1, temp_board);
							stone_num[color % 2 + 1] = stone_num[color % 2 + 1] - take_num;
							createEye(temp, color, temp_board);
							color = color % 2 + 1;

							//printf("take num: %d\n",take_num);

							take_num = 0;

							temp.x = 11;	// TODO. 是换成敌方,但为什么要放弃了这一行,继续给敌方子遍历这一行不好嘛
							break;
						}
						else {
							temp_board[temp.x][temp.y] = 0;
						}
					}
				}
			}
		}
		if (flag == 0) {
			color = color % 2 + 1;
			/*out++;
			if (out == 10)
				break;*/
		}
		//printf("%d\n",num);
	}
	//PrintTempBoard();
	if (stone_num[myColor] + eye[myColor] > stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		return 1;
	if (stone_num[myColor] + eye[myColor] < stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		return -1;
	return 0;
}

bool CMonteCarlo::nbMove(PMove*	pMove)
{
	int i, num, point, max_point = 0;
	PMove temp, begin_stone, best;

	for (begin_stone.x = 1; begin_stone.x <= 9; begin_stone.x++) {
		for (begin_stone.y = 1; begin_stone.y <= 9; begin_stone.y++) {
			boardCopy();
			if (!check(begin_stone, myColor))
				continue;
			point = 0;
			for (i = 0; i < MT_NUMS; i++) {
				srand(rand()*(int)time(0));
				boardCopy();	// TODO.这是因为check改变了棋盘结构, 舍弃是必然的, 但是check究竟是获得了很多信息的,这么样就舍弃掉check获得的数据是否不应该
				temp_board[begin_stone.x][begin_stone.y] = myColor;
				stone_num[myColor] = player[myColor].total + 1;
				stone_num[myColor % 2 + 1] = player[myColor].know;

				// 给敌方随机落子补齐
				while (player[myColor % 2 + 1].total > stone_num[myColor % 2 + 1]) {
					num = 1 + (int)((81.0 - stone_num[1] - stone_num[2])*rand() / (RAND_MAX + 1.0));
					for (temp.x = 1; temp.x <= 9; temp.x++) {
						for (temp.y = 1; temp.y <= 9; temp.y++) {
							if (temp_board[temp.x][temp.y] == 0) {
								num--;
								if (num == 1 && check(temp, myColor % 2 + 1)) {
									temp_board[temp.x][temp.y] = myColor % 2 + 1;
									stone_num[myColor % 2 + 1] ++;
									temp.x = 10;
									break;
								}
							}
						}
					}
				}
				//PrintTempBoard();
				point += MonteCarloMove(myColor % 2 + 1);
			}
			//printf(" Score : %d\n",point);
//			mark[begin_stone.x][begin_stone.y] += point;
			if (point > max_point && begin_stone.x != (*pMove).x && begin_stone.y != (*pMove).y) {
				max_point = point;
				best.x = begin_stone.x;
				best.y = begin_stone.y;
			}
		}
	}
	//	PrintMarkBord();
	if (max_point > TRUST_VALUE) {	
		*pMove = best;
		return true;
	}

	return false;
}
