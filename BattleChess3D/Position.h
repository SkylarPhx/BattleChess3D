#include "stdafx.h"
#include "Pieces.h"
#include "ConsoleColor.h"
using namespace std;

class Position
{
private:
	list<Piece*> whitePieces, blackPieces;
	// Chess board which contais pointers to pieces.
	// If a square doesn't have a piece on it, it'll point to NULL.
	array<array<Piece*, 8>, 8> board;

	enum
	{
		A, B, C, D, E, F, G, H
	};
	
	enum
	{
		R1, R2, R3, R4, R5, R6, R7, R8
	};

public:
	Position()
	{
		// Assigning chess pieces to players.
		whitePieces.push_back(new King(WHITE, R1, E));
		whitePieces.push_back(new Queen(WHITE, R1, D));
		whitePieces.push_back(new Rook(WHITE, R1, A));
		whitePieces.push_back(new Rook(WHITE, R1, H));
		whitePieces.push_back(new Bishop(WHITE, R1, C));
		whitePieces.push_back(new Bishop(WHITE, R1, F));
		whitePieces.push_back(new Knight(WHITE, R1, B));
		whitePieces.push_back(new Knight(WHITE, R1, G));
		whitePieces.push_back(new Pawn(WHITE, R2, A));
		whitePieces.push_back(new Pawn(WHITE, R2, B));
		whitePieces.push_back(new Pawn(WHITE, R2, C));
		whitePieces.push_back(new Pawn(WHITE, R2, D));
		whitePieces.push_back(new Pawn(WHITE, R2, E));
		whitePieces.push_back(new Pawn(WHITE, R2, F));
		whitePieces.push_back(new Pawn(WHITE, R2, G));
		whitePieces.push_back(new Pawn(WHITE, R2, H));
		blackPieces.push_back(new King(BLACK, R8, E));
		blackPieces.push_back(new Queen(BLACK, R8, D));
		blackPieces.push_back(new Rook(BLACK, R8, A));
		blackPieces.push_back(new Rook(BLACK, R8, H));
		blackPieces.push_back(new Bishop(BLACK, R8, C));
		blackPieces.push_back(new Bishop(BLACK, R8, F));
		blackPieces.push_back(new Knight(BLACK, R8, B));
		blackPieces.push_back(new Knight(BLACK, R8, G));
		blackPieces.push_back(new Pawn(BLACK, R7, A));
		blackPieces.push_back(new Pawn(BLACK, R7, B));
		blackPieces.push_back(new Pawn(BLACK, R7, C));
		blackPieces.push_back(new Pawn(BLACK, R7, D));
		blackPieces.push_back(new Pawn(BLACK, R7, E));
		blackPieces.push_back(new Pawn(BLACK, R7, F));
		blackPieces.push_back(new Pawn(BLACK, R7, G));
		blackPieces.push_back(new Pawn(BLACK, R7, H));
		clear();
	}

	~Position()
	{
		for(auto &p: whitePieces)
			delete p;
		
		for(auto &p: blackPieces)
			delete p;
	}

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
		for(auto &p: whitePieces)
		{
			board[p->row][p->col] = p;
		}
		for(auto &p: blackPieces)
		{
			board[p->row][p->col] = p;
		}
	}

	void doMove()
	{
	}

	void showPosition() // Debug print.
	{
		bool turn = true;
		cout << "" << endl;
		for(short i=8; i--> 0;)
		{
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
			cout << greyBlack << endl;
		}
	}
};

class Move
{
private:
	Position position;

public:
	Move(){}
	~Move(){}

	void setMove(string command)
	{
	}

	void executeMove()
	{
	}
};
