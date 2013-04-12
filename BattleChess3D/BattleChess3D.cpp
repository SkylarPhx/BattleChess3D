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
		for(auto &m: moves)
		{
			switch(position->whoIsOn(m.fromCol, m.fromRow))
			{
			case KING:
				cout << "King ";
				break;
			case QUEEN:
				cout << "Queen ";
				break;
			case ROOK:
				cout << "Rook ";
				break;
			case BISHOP:
				cout << "Bishop ";
				break;
			case KNIGHT:
				cout << "Knight ";
				break;
			case PAWN:
				cout << "Pawn ";
				break;
			}
			cout << (char)(m.fromCol + 97) << (m.fromRow + 1) << "-" << (char)(m.toCol + 97) << (m.toRow + 1) << endl;
		}
		cout << "\nGive a move command like e2-e4\nRanges: [a:h][1:8]" << endl;
		getline(cin, command, '\n');
		if(command.size() == 5)
		{
			if(command[0] < 97 || command[0] > 104) continue;
			if(command[1] < 49 || command[1] > 56) continue;
			if(command[3] < 97 || command[3] > 104) continue;
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
