// BattleChess3D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Position.h"
using namespace std;

// Normal: 78, max: 206
const short Position::value[6] = { 0, 18, 10, 6, 6, 2 };
// Max: 48
const short Position::center[8][8] = {
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 1 },
	{ 1, 2, 3, 3, 3, 3, 2, 1 },
	{ 1, 2, 3, 3, 3, 3, 2, 1 },
	{ 1, 2, 3, 3, 3, 3, 2, 1 },
	{ 1, 2, 3, 3, 3, 3, 2, 1 },
	{ 1, 2, 2, 2, 2, 2, 2, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 }
};
// Max: 2
const short Position::safety[8][8] = {
	{ 2, 2, 1, 0, 0, 1, 2, 2 },
	{ 2, 1, 0, 0, 0, 0, 1, 2 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 2, 1, 0, 0, 0, 0, 1, 2 },
	{ 2, 2, 1, 0, 0, 1, 2, 2 }
};
// Max: 32
const short Position::promotion[2][8] = {
	{ 0, 3, 1, 0, 0, 0, 0, 0 }, // Black
	{ 0, 0, 0, 0, 0, 1, 3, 0 }  // White
};

int _tmain(int argc, _TCHAR* argv[])
{
	Position position;
	position.start();
	position.showPosition();
	Move move;
	bool cheat = false, moved;

	string command = "";
	while(1)
	{
		// Deletes old position and copies its data to new position.
		Position oldPosition(position);
		position.copyPosition(oldPosition);
		Owner turn = position.tellTurn();
		if(position.isDraw())
		{
			cout << "DRAW!" << endl;
			break;
		}
		list<Move> moves;
		list<Piece*> threats;
		cout << "Legal moves: " << position.generateLegalMoves(moves, threats) << endl;
		if(threats.size()) cout << "Check!" << endl;
		if(moves.empty())
		{
			cout << "GAME ENDED!" << endl;
			break;
		}
		position.showSpecialInfo();
		cout << "Evaluation: " << position.evaluate(0) << endl;

		if(turn == BLACK)
		{
			Move bestMove = position.selectBestMove(moves);
			printMove(bestMove);
			position.executeMove(bestMove);
			moved = true;
		}
		else moved = false;

		while(turn == WHITE)
		{
			/*for(auto &m: moves)
			{
				switch(position.whoIsOn(m.fromCol, m.fromRow))
				{
				case KING: cout << "King "; break;
				case QUEEN: cout << "Queen "; break;
				case ROOK: cout << "Rook "; break;
				case BISHOP: cout << "Bishop "; break;
				case KNIGHT: cout << "Knight "; break;
				case PAWN: cout << "Pawn "; break;
				}
				printMove(m);
			}*/
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
				position.executeMove(move);
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
						position.executeMove(m);
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

		position.showPosition();

		// Turn changes.
		if(moved) position.changeTurn();
	}
	cin.get();
	return 0;
}
