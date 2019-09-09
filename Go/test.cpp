#include "CEngine.h"
#include "bsmcts.h"
#include <windows.h>

int main()
{
	mcts test;
	char	msg[100];
	int		numTi = 0;
	// int		x, y;
	test.InitGame();

	while (1)
	{
		fout.open("D:/2.txt", std::ios::app);
		Sleep(50);
		fflush(stdin);
		Sleep(50);
		fflush(stdout);
		scanf("%s", msg);
		fout << "I " << msg << "\n";
		if (strcmp(msg, "accept") == 0)
		{
			fflush(stdin);
			test.MoveSucceed();
			/*if(test.player[test.myColor].know > 5)
				test.prestate = test.opchoice(test.bestmove);*/
		}
		else if (strcmp(msg, "refuse") == 0)
		{
			fflush(stdin);
			test.MoveFail();
			test.Move();
		}
		else if (strcmp(msg, "move") == 0)
		{
			scanf("%s", msg); // Read "go"
			fout << "I " << msg << "\n";
			fflush(stdin);
			test.player[test.opColor].total++;
			test.Move();
		}
		else if (strcmp(msg, "access") == 0) //己方提子
		{

			scanf("%s", msg); // Read "take"
			fout << "I " << msg << "\n";
			scanf("%d%s", &numTi, msg);
			fout << "I " << numTi << "\n";
			fout << "I " << msg << "\n";
			fflush(stdin);
			test.player[test.opColor].total -= numTi;
			//SMove test.MoveList[81];
			for (int i = 0; i < numTi; i++)
			{
				// test.MoveList[i].x = msg[2 * i] - 'A' + 1;
				// test.MoveList[i].y = msg[2 * i + 1] - 'A' + 1;
				// test.board[test.MoveList[i].x][test.MoveList[i].y] = test.NOSTONE;
				char x = msg[2 * i] - 'A' + 1;
				char y = msg[2 * i + 1] - 'A' + 1;
				test.board[x][y] = test.NOSTONE;
			}
		}
		else if (strcmp(msg, "taked") == 0) //对方提子
		{
			scanf("%d%s", &numTi, msg);
			fout << "I " << numTi << "\n";
			fout << "I " << msg << "\n";
			test.player[test.myColor].total -= numTi;
			//SMove test.MoveList[81] ;
			for (int i = 0; i < numTi; i++)
			{
				// x = test.MoveList[i].x = msg[2 * i] - 'A' + 1;
				// y = test.MoveList[i].y = msg[2 * i + 1] - 'A' + 1;
				char x = msg[2 * i] - 'A' + 1;
				char y = msg[2 * i + 1] - 'A' + 1;
				test.board[x][y] = test.NOSTONE;

				//对所提的子的边界进行探测，如果是空，肯定是对方的子
				if (test.board[x - 1][y] == test.NOSTONE)
				{
					test.board[x - 1][y] = test.opColor;
					test.player[test.myColor].know++;
				}
				if (test.board[x + 1][y] == test.NOSTONE)
				{
					test.board[x + 1][y] = test.opColor;
					test.player[test.myColor].know++;
				}
				if (test.board[x][y - 1] == test.NOSTONE)
				{
					test.board[x][y - 1] = test.opColor;
					test.player[test.myColor].know++;
				}
				if (test.board[x][y + 1] == test.NOSTONE)
				{
					test.board[x][y + 1] = test.opColor;
					test.player[test.myColor].know++;
				}
			}
		}
		else if (strcmp(msg, "name?") == 0)
		{
			fflush(stdin);
			printf("name YuckyGo\n");
			fflush(stdout);
		}
		else if (strcmp(msg, "end") == 0)
		{
			break;
		}
		else if (strcmp(msg, "quit") == 0)
		{
			break;
		}
		else if (strcmp(msg, "new") == 0)
		{
			test.InitGame();
			scanf("%s", msg);
			fout << "I " << msg << "\n";
			fflush(stdin);
			if (strcmp(msg, "black") == 0)
			{
				test.myColor = test.BLACK;
				test.opColor = test.WHITE;
				test.SearchMove(&test.bestMove);
				printf("move %c%c\n", test.bestMove.x + 'A' - 1, test.bestMove.y + 'A' - 1);
				fout << "O " << "move " << char(test.bestMove.x + 'A' - 1) << char(test.bestMove.y + 'A' - 1) << "\n";
				fflush(stdout);
			}
			else
			{
				test.myColor = test.WHITE;
				test.opColor = test.BLACK;
			}
		}
		else
		{
			printf("Error Input!!!\n");
			test.BoardShow();
		}
		fout.close();
		test.BoardShow();
	}

	return 0;
}