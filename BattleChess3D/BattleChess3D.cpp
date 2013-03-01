// BattleChess3D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Position.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Position *position = new Position();
	position->start();
	position->showPosition();
	Move move;

	string command = "";
	while(1)
	{
		//int64_t duration = 0 - chrono::system_clock::to_time_t(chrono::system_clock::now());

		// Deletes old position and copies its data to new position.
		Position oldPosition(*position);
		delete position;
		position = new Position(oldPosition);
		list<Move> moves;
		cout << "Legal moves: " << position->generateLegalMoves(moves) << endl;
		cout << "\nGive a move command like E2-E4\nRanges: [A:H][1:8]" << endl;
		getline(cin, command, '\n');
		if(command.size() == 5)
		{
			if(command[0] < 65 || command[0] > 72) continue;
			if(command[1] < 49 || command[1] > 56) continue;
			if(command[3] < 65 || command[3] > 72) continue;
			if(command[4] < 49 || command[4] > 56) continue;
			move.setMove(command);
			position->executeMove(move);
		}

		//this_thread::sleep_for(chrono::milliseconds(500));
		position->showPosition();
		
		// Turn changes.
		position->changeTurn();

		//duration += chrono::system_clock::to_time_t(chrono::system_clock::now());
		//cout << duration << " seconds this turn." << endl;
	}

	return 0;
}
