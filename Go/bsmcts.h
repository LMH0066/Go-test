#include<iostream>
#include"CMonteCarlo.h"
#include<cmath>
#include<vector> 
using namespace std;
class mcts :
	public CMonteCarlo
{
public:
	class node
	{
	public:
		node();
		double	value;
		PMove   nmove;
		node	*parent_node;		//有没有必要记录节点的父子关系？ 
		vector<node>	child_node;
	};

	class state
	{
	public:
		state();
		int		color;
		int		visit_times;
		double	ucb;
		double	vastate;
		double	pro;
		PMove	pmove;						//信念状态对应的走法 
		state	*parent_state;				//父信念状态 
		vector<node>	prenode;			//当前拥有的根历史 
		vector<state*>	child_state;		//子信念状态 

		void	compute_va(bool result, int mycolor);	//收益 
		void	compute_ucb();				//ucb值 
		void	compute_pro();				//对手预测中的可能性 
	};
	~mcts();
	int computation_budget = 4000;			//循环次数
	double va_board[BOARD_ROWS][BOARD_ROWS][2] = { 0 };
	bool first = true;
	node history;							//当前树搜索过程中的结点 		
	char tree_board[BOARD_ROWS][BOARD_ROWS];//树搜索过程中所走的棋盘每次迭代调用board_copy更新 
	state *prestate;	//指针						//当前根信念状态
	state *ancestor;
	void  new_node(state *cstate);//done
	virtual PMove *search(PMove* pMove);//指针			//核心调用函数，cstate为当前处理的信念状态 
	state *new_state(state *cstate, PMove smove);//done//函数传值问题 
	void back_up(state **cstate, bool result);//指针//回溯函数 
	state *mychoice(state *cstate);	//done		//我对于信念状态转移的选择 
	state *opchoice(state *cstate);	//done		//对手对于信念状态转移的选择 
	bool expand(state **cstate);	//指针			//从根信念状态扩张，包含选择与拓展 
	double simulate(state &cstate);	//done		//模拟 
	state *choosebest();						//最终选择收益最大的走法 
	bool unexplored(state *cstate, PMove temp);//done//判断未建立该新节点 
	int chosen(state *cstate, PMove temp);	//done//判断已建立该信念状态，返回状态编号，未建立则返回-1 
	void board_copy();						//将已走棋盘复制到tree_board
	bool is_all_expand(state *cstate);//done
	int MCMove(char color, int stone_num[4], char temp_board[BOARD_ROWS][BOARD_ROWS]);//就加了个传值，以及返回值我把它改成只有1和0
	void Init_Prestate();
	state *bestmove;

	char Forbidden[BOARD_ROWS][BOARD_ROWS];

};
