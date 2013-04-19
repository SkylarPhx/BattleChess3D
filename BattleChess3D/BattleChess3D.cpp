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
	bool cheat = false, moved;

	string command = "";
	while(1)
	{
		//int64_t duration = 0 - chrono::system_clock::to_time_t(chrono::system_clock::now());

		// Deletes old position and copies its data to new position.
		Position *oldPosition = new Position(*position);
		delete position;
		position = new Position(*oldPosition);
		delete oldPosition;
		position->tellTurn();
		list<Move> moves;
		cout << "Legal moves: " << position->generateLegalMoves(moves) << endl;
		if(moves.size() == 0) cout << "GAME ENDED!" << endl;
		/*for(auto &m: moves)
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
		}*/
		position->showSpecialInfo();
		cout << "MinMax: " << position->evaluate() << endl;
		position->selectBestMove(moves);

		moved = false;
		while(true)
		{
			cout << "\nGive a move command like e2-e4\nRanges: [a:h][1:8]" << endl;
			getline(cin, command, '\n');
			if(command == "exit") return 0;
			if(command.size() <= 4) continue;
			if(command == "cheat")
			{
				cheat = !cheat;
				if(cheat)
					cout << "Cheating" << endl;
				else
					cout << "Normal playing" << endl;
				continue;
			}

			if(command[0] < 97 || command[0] > 104) continue;
			if(command[1] < 49 || command[1] > 56) continue;
			if(command[3] < 97 || command[3] > 104) continue;
			if(command[4] < 49 || command[4] > 56) continue;
			move.setMove(command);

			if(cheat)
			{
				if(command.size() > 6)
				move.setSpecial(command[6]);
				position->executeMove(move);
				moved = true;
			}
			else
			{
				bool legal = false;
				for(Move &m: moves)
				{
					if(m.fromCol == move.fromCol
						&& m.fromRow == move.fromRow
						&& m.toCol == move.toCol
						&& m.toRow == move.toRow)
					{
						legal = true;
						m.AI = false;
						position->executeMove(m);
						moved = true;
						break;
					}
				}
				if(!legal)
				{
					cout << "Illegal move!" << endl;
				}
			}
			break;
		}

		//this_thread::sleep_for(chrono::milliseconds(500));
		position->showPosition();

		// Turn changes.
		if(moved)
			position->changeTurn();

		//duration += chrono::system_clock::to_time_t(chrono::system_clock::now());
		//cout << duration << " seconds this turn." << endl;
	}

	return 0;
}
