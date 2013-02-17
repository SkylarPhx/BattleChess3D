// BattleChess3D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Position.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Position position;
	position.start();
	position.showPosition();
	Move move;

	string command = "";
	while(1)
	{
		cout << "\nGive a move command like E2-E4\nRanges: [A:H][1:8]" << endl;
		getline(cin, command, '\n');
		if(command.size() == 5)
		{
			move.setMove(command);
			position.executeMove(move);
		}

		this_thread::sleep_for(chrono::milliseconds(500));
		position.showPosition();
	}

	return 0;
}
