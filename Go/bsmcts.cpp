#include"bsmcts.h"

mcts::PMove* mcts::search(PMove* pMove)
{
	if (first) {
		Init_Prestate();
		first = false;
	}
	while (true)
	{
		for (int i = 0; i < computation_budget; i++)
		{
			bool result;//result为单次模拟结果 
			board_copy();//复制已知棋盘 
			tree_board[prestate->pmove.x][prestate->pmove.y] = prestate->color;
			expand(&prestate);
			/*if (i%100 == 0)
			{
				printf("-------------------\n");
				PrintTempBoard(tree_board);
				printf("-------------------\n");
			}*/
			result = simulate(*prestate);
			back_up(&prestate, result);
		}

		bestmove = choosebest();
		if (!IsInBoard(bestmove->pmove.x, bestmove->pmove.y))
		{
			break;
		}
	}
	pMove->x = bestmove->pmove.x;
	pMove->y = bestmove->pmove.y;
	return &bestmove->pmove;
}

mcts::node::node()
{
	value = 0;
	parent_node = NULL;
}

mcts::state::state()
{
	ucb = 0;
	visit_times = 0;
	vastate = 0;
	parent_state = NULL;
}

bool mcts::expand(state **cstate)
{
	srand(rand()*(int)time(0));
	int num;
	int stone_num[4] = { 0 };
	stone_num[myColor] = player[myColor].total;
	stone_num[opColor] = player[opColor].total;


	do
	{
		num = rand() % ((*cstate)->prenode.size());
		history = (*cstate)->prenode[num];//创建根信念状态的历史 
	} while (IsInBoard(history.nmove.x, history.nmove.y));//补加了一个判断是否已经在棋盘上的判断

	int number = 0;
	while (is_all_expand(*cstate))
	{
		if (!(*cstate)->child_state.size() || number >= player[opColor].total)	//补充：如果自状态为空（即已经到底部）或者是已经扩展到对方下的子数时，即可推出扩展，进行模拟
		{
			return false;
		}
		if ((*cstate)->color == opColor)
		{
			number++;
			*cstate = mychoice(*cstate);
			tree_board[(*cstate)->pmove.x][(*cstate)->pmove.y] = (*cstate)->color;
			stone_num[(*cstate)->color]++;
		}
		else if ((*cstate)->color == myColor)
		{
			(*cstate) = opchoice(*cstate);
			tree_board[(*cstate)->pmove.x][(*cstate)->pmove.y] = (*cstate)->color;
			stone_num[(*cstate)->color]++;
		}
	}
	PMove temp;//临时用于拓展子状态 
	bool flag = false;

	while (!flag)
	{
		srand(rand()*(int)time(0));
		num = 81.0 - stone_num[1] - stone_num[2] - eye[1] - eye[2];	//随机拓展招法即子状态
		for (int i = 0; i < (*cstate)->child_state.size(); i++)
		{
			if (!IsInBoard((*cstate)->child_state[i]->pmove.x, (*cstate)->child_state[i]->pmove.y, tree_board))
			{
				num--;
				if (num < 0)
				{
					return false;
				}
			}
		}
		num = 1 + (int)(num * rand() / (RAND_MAX + 1.0));
		//num = 1 + (int)((81.0 - stone_num[1] - stone_num[2] - eye[1] - eye[2] - (*cstate)->child_state.size())*rand() / (RAND_MAX + 1.0));//随机拓展招法即子状态
		for (temp.x = 1; temp.x <= 9; temp.x++) {
			for (temp.y = 1; temp.y <= 9; temp.y++) {
				if (tree_board[temp.x][temp.y] == 0) {
					num--;
					char t_board[BOARD_ROWS][BOARD_ROWS] = { 0 };	//check()会改变传过去的棋盘，我觉着应该再核实一下check()是否有逻辑错误
					for (int i = 0; i < 11; i++)
					{
						for (int j = 0; j < 11; j++)
						{
							t_board[i][j] = tree_board[i][j];
						}
					}
					if (num <= 1 && check(temp, (*cstate)->color % 2 + 1, t_board) && unexplored(*cstate, temp)) {
						flag = true;
						tree_board[temp.x][temp.y] = (*cstate)->color % 2 + 1;
						//stone_num[(*cstate)->color % 2 + 1] ++;
						break;
					}
				}
			}
			if (flag) break;
		}
	}
	// printf("%d\n%d\n", temp.x, temp.y);
	int id = chosen(*cstate, temp);
	if (id == -1)
	{
		// printf("%d\n", *cstate);
		*cstate = new_state(*cstate, temp);
		// printf("%d\n", *cstate);
		// printf("%d\n", &ancestor->child_state[0]);
		// new_node(*(*cstate));
	}
	else if (unexplored(*cstate, temp)) {
		*cstate = (*cstate)->child_state[id];
		new_node(*cstate);
	}
	else {
		*cstate = (*cstate)->child_state[id];
		return false;
	}
	return true;
}


double mcts::simulate(state &cstate)
{
	bool result = 0;
	int stone_num[4] = { 0 };
	char temp_board[BOARD_ROWS][BOARD_ROWS] = {0};
	int know = player[myColor].know;
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 10; j++) {
			temp_board[i][j] = tree_board[i][j];
			if (tree_board[i][j] == myColor) stone_num[myColor]++;//已知棋子+树中已走子 
			if (tree_board[i][j] == opColor) stone_num[opColor]++;
		}
	}
	//随机创建根信念以上的历史 
	int num;
	PMove temp;
	srand(rand()*(int)time(0));
	while (player[opColor].total > know) {	//TODO. C
		num = 1 + (int)((81.0 - stone_num[1] - stone_num[2])*rand() / (RAND_MAX + 1.0));
		for (temp.x = 1; temp.x <= 9; temp.x++) {
			for (temp.y = 1; temp.y <= 9; temp.y++) {
				if (temp_board[temp.x][temp.y] == 0) {
					num--;
					if (num == 1 && check(temp, opColor, temp_board)) {
						temp_board[temp.x][temp.y] = opColor;
						//stone_num[opColor] ++;
						know++;
						temp.x = 10;
						break;
					}
					else if (num < 1) temp.x = temp.y = 10;
				}
			}

		}
	}
	//PrintTempBoard(temp_board);
	result = MCMove(cstate.color % 2 + 1, stone_num, temp_board);//随机走子,改版MonteCarloMove函数 
	return result;
}


void mcts::back_up(state **cstate, bool result)
{
	if (va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0])
	{
		(*cstate)->vastate = va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][1] / va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0];
	}
	else (*cstate)->vastate = 0.1;
	va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0]++;
	(*cstate)->visit_times++;
	(*cstate)->compute_va(result, myColor);
	va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][1] += (*cstate)->vastate;
	// printf("%d\n", *cstate);
	while ((*cstate) != ancestor)
	{
		(*cstate) = (*cstate)->parent_state;
		if (va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0])
		{
			(*cstate)->vastate = va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][1] / va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0];
		}
		else (*cstate)->vastate = 0.1;
		va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][0]++;
		(*cstate)->visit_times++;
		(*cstate)->compute_va(result, myColor);
		va_board[(*cstate)->pmove.x][(*cstate)->pmove.y][1] += (*cstate)->vastate;
		// printf("%d\n", (*cstate)->child_state[0]);
	}
}

mcts::state* mcts::mychoice(state *cstate)
{
	int m_size = cstate->child_state.size();
	if (!m_size)
	{
		return cstate;
	}
	for (int i = 0; i < m_size; i++) {
		cstate->child_state[i]->compute_ucb();
	}
	state *best_next = cstate->child_state[0];
	double best_ucb = cstate->child_state[0]->ucb;
	int i;
	for (i = 0; i < m_size; ++i)
	{
		PMove move = cstate->child_state[i]->pmove;
		if (tree_board[move.x][move.y] == NOSTONE)
		{
			best_next = cstate->child_state[i];
			best_ucb = cstate->child_state[i]->ucb;
			break;
		}
	}
	for (; i < m_size; ++i)
	{
		PMove move = cstate->child_state[i]->pmove;
		if (tree_board[move.x][move.y] == NOSTONE && cstate->child_state[i]->ucb > best_ucb)
		{
			best_next = cstate->child_state[i];
			best_ucb = cstate->child_state[i]->ucb;
		}
	}
	return best_next;
}

mcts::state *mcts::opchoice(state *cstate)
{
	int m_size = cstate->child_state.size();
	if (!m_size)
	{
		return cstate;
	}
	if (cstate != ancestor) cstate->compute_pro();
	for (int i = 0; i < m_size; ++i)
	{
		if (is_all_expand(cstate->child_state[i]))
		{
			cstate->child_state[i]->compute_pro();
		}
	}
	state *best_next = cstate->child_state[0];
	double best_pro = cstate->child_state[0]->pro;
	int i;
	for (i = 0; i < m_size; ++i)
	{
		PMove move = cstate->child_state[i]->pmove;
		if (tree_board[move.x][move.y] == NOSTONE)
		{
			best_next = cstate->child_state[i];
			best_pro = cstate->child_state[i]->pro;
			break;
		}
	}
	for (; i < m_size; ++i)
	{
		PMove move = cstate->child_state[i]->pmove;
		if (tree_board[move.x][move.y] == NOSTONE && cstate->child_state[i]->vastate > best_pro)
		{
			best_next = cstate->child_state[i];
			best_pro = cstate->child_state[i]->pro;
		}
	}
	return best_next;
}

mcts::state *mcts::choosebest()
{
	state *best_next = prestate->child_state[0];
	double best_va = prestate->child_state[0]->vastate;
	for (int i = 0; i < prestate->child_state.size(); ++i)
	{
		if (board[prestate->child_state[i]->pmove.x][prestate->child_state[i]->pmove.y] != NOSTONE 
			|| prestate->child_state[i]->visit_times < 2) continue;
		if (prestate->child_state[i]->vastate > best_va)
		{
			best_next = prestate->child_state[i];
			best_va = prestate->child_state[i]->vastate;
		}
	}
	return best_next;
}

int mcts::chosen(state *cstate, PMove temp)
{
	for (int i = 0; i < cstate->child_state.size(); ++i)
	{
		if (cstate->child_state[i]->pmove.x == temp.x && cstate->child_state[i]->pmove.y == temp.y) return i;
	}
	return -1;
}

bool mcts::unexplored(state *cstate, PMove temp)
{
	int id;
	id = chosen(cstate, temp);
	if (id == -1) return 1;
	else
	{
		for (int i = 0; i < cstate->child_state[id]->prenode.size(); ++i)
		{
			if (history.nmove.x == cstate->child_state[id]->prenode[i].nmove.x && history.nmove.y == cstate->child_state[id]->prenode[i].nmove.y) return 0;
		}
		return 1;
	}
}

void mcts::state::compute_va(bool result, int mycolor)
{
	if (color == mycolor)
		vastate = (vastate*(visit_times - 1) + result) / visit_times;
	else
		vastate = (vastate*(visit_times - 1) - result) / visit_times;
}

void mcts::state::compute_pro()
{
	
	double lamuda = 10;
	double sum = 0;
	for (int i = 0; i < (parent_state->child_state).size(); ++i)
	{
		sum += exp(lamuda*(parent_state->child_state[i]->vastate));
	}
	pro = exp(lamuda*vastate) / sum;
	//pro = (1 - vastate) + 1 / sqrt(2) * sqrt(log(parent_state->visit_times) / visit_times);

}

void mcts::state::compute_ucb()
{
	ucb = vastate + 1 / sqrt(2)*sqrt(log(parent_state->visit_times) / visit_times);
}

void mcts::board_copy()
{
	int i, j;
	for (i = 0; i <= 10; i++) {
		for (j = 0; j <= 10; j++) {
			tree_board[i][j] = board[i][j];
		}
	}
}

mcts::state* mcts::new_state(state *cstate, PMove smove)
{
	state *newstate = new state;
	newstate->color = cstate->color % 2 + 1;
	newstate->pmove = smove;
	newstate->parent_state = cstate;
	newstate->prenode.push_back(history);
	(*cstate).child_state.push_back(newstate);
	// printf("%d\n", &cstate.child_state[0]);
	return newstate;
}

mcts::~mcts()
{
	delete ancestor;
	ancestor = NULL;
}

void mcts::new_node(state *cstate)//是否保存父子关系创建？
{
	cstate->prenode.push_back(history);
}

bool mcts::is_all_expand(state *cstate)
{
	int ch_si = 0;
	int chess_color[3] = { 0 };

	memset(eye, 0, sizeof(eye));
	haveEye(tree_board);

	for (int i = 1; i < 10; i++)
		for (int j = 1; j < 10; j++)
		{
			if (tree_board[i][j] == myColor) chess_color[myColor]++;
			if (tree_board[i][j] == opColor) chess_color[opColor]++;
		}
	if (cstate->child_state.empty())ch_si = 0;
	else {
		ch_si = cstate->child_state.size();
	}
	// printf("child_state.size: %d\nchess_color: %d\nplayer[cstate.color].total: %d", cstate->child_state.size(), chess_color[cstate->color % 2 + 1], player[cstate->color].total);
	if (ch_si + chess_color[cstate->color % 2 + 1] + chess_color[cstate->color] + eye[1] + eye[2] >= 77) return 1;
	// TODO. 感觉可以变为 ch_si , stone_num
	else return 0;
}

//改版蒙特卡洛模拟，就加了个传值 
int mcts::MCMove(char color, int stone_num[4], char temp_board[BOARD_ROWS][BOARD_ROWS])
{
	int num, flag, take_num, out = 0;
	PMove temp;
	memset(eye, 0, sizeof(eye));
	haveEye(temp_board);
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
						if (check(temp, color, temp_board)) {
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

							temp.x = 11;
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
	//PrintTempBoard(temp_board);
	if (stone_num[myColor] + eye[myColor] > stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		return 1;
	/*if (stone_num[myColor] + eye[myColor] < stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		return -1;*/
	return 0;
}

void mcts::Init_Prestate()
{
	srand(rand()*(int)time(0));
	ancestor = new state();
	ancestor->color = opColor;
	int num = 0;
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			if (board[i][j] != myColor && board[i][j] != opColor &&  //补了一个不等于对手的棋子
				(board[i][j + 1] == opColor || board[i][j - 1] == opColor || board[i + 1][j] == opColor || board[i - 1][j] == opColor)) {
				if (rand() % 2) {
					num++;
					node m;
					m.nmove.x = i;
					m.nmove.y = j;
					ancestor->prenode.push_back(m);
				}
			}
		}
	}
	ancestor->pmove = ancestor->prenode[rand() % num].nmove;
	prestate = ancestor;
}
