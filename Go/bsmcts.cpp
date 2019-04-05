#include"bsmcts.h"

mcts::PMove mcts::search(state &cstate)
{
	int computation_budget = 2000;
	for (int i = 0; i < computation_budget; i++)
	{
		bool result;//result为单次模拟结果 
		board_copy();//复制已知棋盘 
		tree_board[cstate.pmove.x][cstate.pmove.y] = cstate.color;
		expand(cstate);
		result = simulate(cstate);
		back_up(cstate, result);
	}
	return choosebest();
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

void mcts::expand(state &cstate)
{
	srand(rand()*(int)time(0));
	int num;
	int stone_num[4];
	stone_num[myColor] = player[myColor].total;
	stone_num[opColor] = player[opColor].total;
	num = rand() % (cstate.prenode.size());
	node history;
	for (int i = 0;; i++)//创建根信念状态历史 
	{
		if (num <= 0) { history = cstate.prenode[i]; break; }
		else num--;
	}
	while (cstate.is_all_expand())//选择方式？ 
	{
		if (cstate.color == opColor)
		{
			cstate = mychoice(cstate);
			tree_board[cstate.pmove.x][cstate.pmove.y] = cstate.color;
		}
		if (cstate.color == myColor)
		{
			cstate = opchoice(cstate);
			tree_board[cstate.pmove.x][cstate.pmove.y] = cstate.color;
		}
	}
	PMove temp;//临时用于拓展子状态 
	bool flag = 0;
	while (!flag)
	{
		num = 1 + (int)((81.0 - stone_num[1] - stone_num[2])*rand() / (RAND_MAX + 1.0));//随机拓展招法即子状态 
		for (temp.x = 1; temp.x <= 9; temp.x++) {
			for (temp.y = 1; temp.y <= 9; temp.y++) {
				if (tree_board[temp.x][temp.y] == 0) {
					num--;
					if (num <= 1 && check(temp, cstate.color % 2 + 1) && unexplored(cstate, temp)) {
						flag = 1;
						tree_board[temp.x][temp.y] = cstate.color % 2 + 1;
						stone_num[cstate.color % 2 + 1] ++;
						temp.x = 10;
						break;
					}
				}
			}
		}
	}
	if (chosen(cstate, temp) == -1)
	{
		cstate = new_state(cstate, temp);
		new_node(cstate);
	}
	else
		new_node(cstate);
}


double mcts::simulate(state &cstate)
{
	int result;
	int stone_num[4];
	int know;
	char temp_board[BOARD_ROWS][BOARD_ROWS];
	know = player[myColor].know;
	result = 0;
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
	while (player[opColor].total > know) {
		num = 1 + (int)((81.0 - stone_num[1] - stone_num[2])*rand() / (RAND_MAX + 1.0));
		for (temp.x = 1; temp.x <= 9; temp.x++) {
			for (temp.y = 1; temp.y <= 9; temp.y++) {
				if (temp_board[temp.x][temp.y] == 0) {
					num--;
					if (num == 1 && check(temp, opColor)) {
						temp_board[temp.x][temp.y] = opColor;
						stone_num[opColor] ++;
						know++;
						temp.x = 10;
						break;
					}
				}
			}
		}
	}
	//PrintTempBoard();
	result = MCMove(cstate.color % 2 + 1, stone_num, temp_board);//随机走子,改版MonteCarloMove函数 
}


void mcts::back_up(state &cstate, bool result)
{
	cstate = &(cstate.parent_state);
	cstate.visit_times++;
	cstate.compute_va(result);
	while (cstate != prestate)
	{
		cstate = &(cstate.parent_state);
		cstate.visit_times++;
		cstate.compute_va(result);
	}
}

mcts::state mcts::mychoice(state &cstate)
{
	cstate.compute_ucb();
	state best_next = cstate.child_state[0];
	double best_ucb = cstate.child_state[0].ucb;
	for (int i = 0; i < cstate.child_state.size(); ++i)
	{
		if (cstate.child_state[i].ucb > best_ucb)
		{
			best_next = cstate.child_state[i];
			best_ucb = cstate.child_state[i].ucb;
		}
	}
	return best_next;
}

mcts::state mcts::opchoice(state &cstate)
{
	cstate.compute_pro();
	state best_next=cstate.child_state[0];
	double best_pro=cstate.child_state[0].pro;
	for (int i = 0; i < cstate.child_state.size(); ++i)
	{
		if (cstate.child_state[i].vastate > best_pro)
		{
			best_next = cstate.child_state[i];
			best_pro = cstate.child_state[i].pro;
		}
	}
	return best_next;
}

mcts::PMove mcts::choosebest()
{
	state best_next = prestate.child_state[0];
	double best_va = prestate.child_state[0].vastate;
	for (int i = 0; i < prestate.child_state.size(); ++i)
	{
		if (prestate.child_state[i].vastate > best_va)
		{
			best_next = prestate.child_state[i];
			best_va = prestate.child_state[i].vastate;
		}
	}
	return best_next.pmove;
}

int mcts::chosen(state &cstate,PMove temp)
{
	for(int i=0;i<cstate.child_state.size();++i)
	{
		if(cstate.child_state[i].pmove.x==temp.x && cstate.child_state[i].pmove.y == temp.y) return i;
	}
	return -1;
}

bool mcts::unexplored(state &cstate, PMove temp)
{
	int id;
	id = chosen(cstate, temp);
	if (id == -1) return 1;
	else
	{
		for (int i = 0; i < cstate.child_state[id].prenode.size(); ++i)
		{
			if () return 0;//如何判断是否生成该历史的对应结点 ?
		}
		return 1;
	}
}

void mcts::state::compute_va(bool result)
{
	if (color == myColor)
		vastate = (vastate*(visit_times - 1) + result) / visit_times;
	else
		vastate = (vastate*(visit_times - 1) + !result) / visit_times;
}

void mcts::state::compute_pro()
{
	double lamuda = 0.5;
	double sum;
	for (int i = 0; i < (parent_state->child_state).size(); ++i)
	{
		sum += exp(lamuda*(parent_state->child_state[i].vastate));
	}
	pro = exp(lamuda*vastate) / sum;
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

mcts::state mcts::new_state(state &cstate, PMove smove)
{
	state newstate;
	newstate.color = cstate.color % 2 + 1;
	newstate.pmove = smove;
	newstate.parent_state = &cstate;
	return newstate;
}

void mcts::state::new_node(state &cstate)//是否保存父子关系创建？ 
{
	
}

//改版蒙特卡洛模拟，就加了个传值 
int mcts::MCMove(char color, int stone_num[4], char temp_board[BOARD_ROWS][BOARD_ROWS])
{
	int num, flag, take_num, out = 0;
	PMove temp;
	int eye[4];
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
	//PrintTempBoard();
	if (stone_num[myColor] + eye[myColor] > stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		return 1;
	//if (stone_num[myColor] + eye[myColor] < stone_num[myColor % 2 + 1] + eye[myColor % 2 + 1])
		//return -1;
	return 0;
}