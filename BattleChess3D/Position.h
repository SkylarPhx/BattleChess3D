#include "stdafx.h"
#include "Pieces.h"
#include "ConsoleColor.h"
using namespace std;

class Move
{
public:
	short fromRow, fromCol, toRow, toCol;

	Move(){}
	~Move(){}

	void setMove(string &command)
	{
		fromCol = command[0] - 65;
		fromRow = command[1] - 49;
		toCol = command[3] - 65;
		toRow = command[4] - 49;
	}
};

class Position
{
private:
	list<Piece> whitePieces, blackPieces;
	// Chess board which contais pointers to pieces.
	// If a square doesn't have a piece on it, it'll point to NULL.
	array<array<Piece*, 8>, 8> board;

public:
	Position(){}
	Position(Position &p)
	{
		copyPosition(p);
	}

	~Position(){}

	// Removes all Pieces from the chess board.
	void clear()
	{
		for(auto &row: board)
		{
			for(auto &p: row)
			{
				p = NULL;
			}
		}
	}

	// Puts pieces in starting position.
	void start()
	{
		clear();
		// Assigning chess pieces to players.
		whitePieces.emplace_back(WHITE, KING, R1, E);
		whitePieces.emplace_back(WHITE, QUEEN, R1, D);
		whitePieces.emplace_back(WHITE, ROOK, R1, A);
		whitePieces.emplace_back(WHITE, ROOK, R1, H);
		whitePieces.emplace_back(WHITE, BISHOP, R1, C);
		whitePieces.emplace_back(WHITE, BISHOP, R1, F);
		whitePieces.emplace_back(WHITE, KNIGHT, R1, B);
		whitePieces.emplace_back(WHITE, KNIGHT, R1, G);
		whitePieces.emplace_back(WHITE, PAWN, R2, A);
		whitePieces.emplace_back(WHITE, PAWN, R2, B);
		whitePieces.emplace_back(WHITE, PAWN, R2, C);
		whitePieces.emplace_back(WHITE, PAWN, R2, D);
		whitePieces.emplace_back(WHITE, PAWN, R2, E);
		whitePieces.emplace_back(WHITE, PAWN, R2, F);
		whitePieces.emplace_back(WHITE, PAWN, R2, G);
		whitePieces.emplace_back(WHITE, PAWN, R2, H);

		for(auto &p: whitePieces)
		{
			board[p.row][p.col] = &p;
		}

		blackPieces.emplace_back(BLACK, KING, R8, E);
		blackPieces.emplace_back(BLACK, QUEEN, R8, D);
		blackPieces.emplace_back(BLACK, ROOK, R8, A);
		blackPieces.emplace_back(BLACK, ROOK, R8, H);
		blackPieces.emplace_back(BLACK, BISHOP, R8, C);
		blackPieces.emplace_back(BLACK, BISHOP, R8, F);
		blackPieces.emplace_back(BLACK, KNIGHT, R8, B);
		blackPieces.emplace_back(BLACK, KNIGHT, R8, G);
		blackPieces.emplace_back(BLACK, PAWN, R7, A);
		blackPieces.emplace_back(BLACK, PAWN, R7, B);
		blackPieces.emplace_back(BLACK, PAWN, R7, C);
		blackPieces.emplace_back(BLACK, PAWN, R7, D);
		blackPieces.emplace_back(BLACK, PAWN, R7, E);
		blackPieces.emplace_back(BLACK, PAWN, R7, F);
		blackPieces.emplace_back(BLACK, PAWN, R7, G);
		blackPieces.emplace_back(BLACK, PAWN, R7, H);

		for(auto &p: blackPieces)
		{
			board[p.row][p.col] = &p;
		}
	}

	void copyPosition(Position &p)
	{
		clear();

		whitePieces = p.whitePieces;
		for(auto &p: whitePieces)
		{
			board[p.row][p.col] = &p;
		}

		blackPieces = p.blackPieces;
		for(auto &p: blackPieces)
		{
			board[p.row][p.col] = &p;
		}
	}

	void executeMove(Move &m)
	{
		Piece* to = board[m.toRow][m.toCol];
		if(to != NULL)
		{
			// Delete eatable chess piece.
			list<Piece>* list = (to->owner == BLACK) ? &blackPieces : &whitePieces;
			auto i = (*list).end();
			while(true)
			{
				i--;
				if(i->row == m.toRow && i->col == m.toCol)
				{
					(*list).erase(i);
					break;
				}
				if(i == (*list).begin()) break;
			}
		}
		Piece* from = board[m.fromRow][m.fromCol];
		if(from != NULL)
		{
			// Change the location of movable chess piece.
			from->row = m.toRow;
			from->col = m.toCol;
		}
		// Change pointers to chess pieces on the chess board.
		board[m.toRow][m.toCol] = board[m.fromRow][m.fromCol];
		board[m.fromRow][m.fromCol] = NULL;
	}

	void showPosition() // Debug print.
	{
		bool turn = true;
		cout << "\n    A  B  C  D  E  F  G  H" << endl;
		for(short i=8; i--> 0;)
		{
			cout << " " << (i + 1) << " ";
			for(auto &p: board[i])
			{
				if(p == NULL)
				{
					if(turn)
						cout << blackYellow;
					else
						cout << blackGold;
					cout << "   ";
					turn = !turn;
					continue;
				}
				if(turn)
				{
					if(p->owner == BLACK)
						cout << blackYellow;
					else
						cout << whiteYellow;
				}
				else
				{
					if(p->owner == BLACK)
						cout << blackGold;
					else
						cout << whiteGold;
				}
				turn = !turn;
				p->debugPrint();
			}
			turn = !turn;
			cout << greyBlack << " " << (i + 1) << endl;
		}
		cout << "    A  B  C  D  E  F  G  H" << endl;

		cout << "\n White" << endl;
		for(auto &p: whitePieces)
		{
			p.debugPrint();
			cout << (char)(p.col + 65) << (p.row + 1) << " ";
		}
		
		cout << "\n\n Black" << endl;
		for(auto &p: blackPieces)
		{
			p.debugPrint();
			cout << (char)(p.col + 65) << (p.row + 1) << " ";
		}
		cout << endl;
	}
};
