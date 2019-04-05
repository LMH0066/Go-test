#include<iostream>
#include"CEngine.h"
#include"CMonteCarlo.h"
#include<cmath>
#include<vector> 
using namespace std;
class node
{
	public:
		node();
		double value;
		node *parent_node;//有没有必要记录节点的父子关系？ 
		vector<node> child_node;
		bool is_all_expand(); 
}
class state
{
	public:
		state();
		int color;
		PMove pmove;//信念状态对应的走法 
		state *parent_state; //父信念状态 
		vector<node> prenode;//当前拥有的节点 
		vector<state> child_state;//子信念状态 
		double ucb;
		double vastate;
		double pro;
		int visit_times;
		void new_node(state &cstate);
		void compute_va(bool result);//收益 
		void compute_ucb();//ucb值 
		void compute_pro();//对手预测中的可能性 
}
class mcts
{
	public:
		char tree_board[BOARD_ROWS][BOARD_ROWS];//树搜索过程中所走的棋盘每次迭代调用board_copy更新 
		state prestate;//当前根信念状态 
		PMove search(state &cstate);//核心调用函数，cstate为当前处理的信念状态 
		state new_state(state &cstate,PMove smove);//函数传值问题 
		void back_up(state &cstate,bool result);//回溯函数 
		state mychoice(state &cstate);//我对于信念状态转移的选择 
		state opchoice(state &cstate);//对手对于信念状态转移的选择 
		void expand(state &cstate);//从根信念状态扩张，包含选择与拓展 
		double simulate(state &cstate);//模拟 
		PMove choosebest();//最终选择收益最大的走法 
		bool unexplored(state &cstate,PMove temp);//判断未建立该新节点 
		int chosen(state &cstate,PMove temp);//判断已建立该信念状态，返回状态编号，未建立则返回-1 
		void board_copy();//将已走棋盘复制到tree_board
		int MCMove(char color,int stone_num[4],char temp_board[BOARD_ROWS][BOARD_ROWS]);//就加了个传值，以及返回值我把它改成只有1和0 
}
