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
		node *parent_node;//��û�б�Ҫ��¼�ڵ�ĸ��ӹ�ϵ�� 
		vector<node> child_node;
		bool is_all_expand(); 
}
class state
{
	public:
		state();
		int color;
		PMove pmove;//����״̬��Ӧ���߷� 
		state *parent_state; //������״̬ 
		vector<node> prenode;//��ǰӵ�еĽڵ� 
		vector<state> child_state;//������״̬ 
		double ucb;
		double vastate;
		double pro;
		int visit_times;
		void new_node(state &cstate);
		void compute_va(bool result);//���� 
		void compute_ucb();//ucbֵ 
		void compute_pro();//����Ԥ���еĿ����� 
}
class mcts
{
	public:
		char tree_board[BOARD_ROWS][BOARD_ROWS];//���������������ߵ�����ÿ�ε�������board_copy���� 
		state prestate;//��ǰ������״̬ 
		PMove search(state &cstate);//���ĵ��ú�����cstateΪ��ǰ���������״̬ 
		state new_state(state &cstate,PMove smove);//������ֵ���� 
		void back_up(state &cstate,bool result);//���ݺ��� 
		state mychoice(state &cstate);//�Ҷ�������״̬ת�Ƶ�ѡ�� 
		state opchoice(state &cstate);//���ֶ�������״̬ת�Ƶ�ѡ�� 
		void expand(state &cstate);//�Ӹ�����״̬���ţ�����ѡ������չ 
		double simulate(state &cstate);//ģ�� 
		PMove choosebest();//����ѡ�����������߷� 
		bool unexplored(state &cstate,PMove temp);//�ж�δ�������½ڵ� 
		int chosen(state &cstate,PMove temp);//�ж��ѽ���������״̬������״̬��ţ�δ�����򷵻�-1 
		void board_copy();//���������̸��Ƶ�tree_board
		int MCMove(char color,int stone_num[4],char temp_board[BOARD_ROWS][BOARD_ROWS]);//�ͼ��˸���ֵ���Լ�����ֵ�Ұ����ĳ�ֻ��1��0 
}
