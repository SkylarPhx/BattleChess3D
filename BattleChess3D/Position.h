#include "stdafx.h"
#include "Pawns.h"
using namespace std;

class Position
{
private:
	vector<Piece*> whitePieces, blackPieces;
	// Chess board which contais pointers to pieces.
	// If a square doesn't have a piece on it, it'll point to NULL.
	array<array<Piece*, 8>, 8> board;

	enum
	{
		KING, QUEEN, ROOK1, ROOK2, BISHOP1, BISHOP2, KNIGHT1, KNIGHT2,
		PAWN1, PAWN2, PAWN3, PAWN4, PAWN5, PAWN6, PAWN7, PAWN8
	};

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
		// Creating chess pieces and putting them to players.
		whitePieces.reserve(16);
		blackPieces.reserve(16);
		whitePieces.push_back(new King(WHITE));
		blackPieces.push_back(new King(BLACK));
		whitePieces.push_back(new Queen(WHITE));
		blackPieces.push_back(new Queen(BLACK));
		whitePieces.push_back(new Rook(WHITE));
		blackPieces.push_back(new Rook(BLACK));
		whitePieces.push_back(new Rook(WHITE));
		blackPieces.push_back(new Rook(BLACK));
		whitePieces.push_back(new Bishop(WHITE));
		blackPieces.push_back(new Bishop(BLACK));
		whitePieces.push_back(new Bishop(WHITE));
		blackPieces.push_back(new Bishop(BLACK));
		whitePieces.push_back(new Knight(WHITE));
		blackPieces.push_back(new Knight(BLACK));
		whitePieces.push_back(new Knight(WHITE));
		blackPieces.push_back(new Knight(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
		whitePieces.push_back(new Pawn(WHITE));
		blackPieces.push_back(new Pawn(BLACK));
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
		board[R8][A] = blackPieces[ROOK1];
		board[R8][B] = blackPieces[KNIGHT1];
		board[R8][C] = blackPieces[BISHOP1];
		board[R8][D] = blackPieces[KING];
		board[R8][E] = blackPieces[QUEEN];
		board[R8][F] = blackPieces[BISHOP2];
		board[R8][G] = blackPieces[KNIGHT2];
		board[R8][H] = blackPieces[ROOK2];

		board[R7][A] = blackPieces[PAWN1];
		board[R7][B] = blackPieces[PAWN2];
		board[R7][C] = blackPieces[PAWN3];
		board[R7][D] = blackPieces[PAWN4];
		board[R7][E] = blackPieces[PAWN5];
		board[R7][F] = blackPieces[PAWN6];
		board[R7][G] = blackPieces[PAWN7];
		board[R7][H] = blackPieces[PAWN8];
		
		board[R2][A] = whitePieces[PAWN1];
		board[R2][B] = whitePieces[PAWN2];
		board[R2][C] = whitePieces[PAWN3];
		board[R2][D] = whitePieces[PAWN4];
		board[R2][E] = whitePieces[PAWN5];
		board[R2][F] = whitePieces[PAWN6];
		board[R2][G] = whitePieces[PAWN7];
		board[R2][H] = whitePieces[PAWN8];
		
		board[R1][A] = whitePieces[ROOK1];
		board[R1][B] = whitePieces[KNIGHT1];
		board[R1][C] = whitePieces[BISHOP1];
		board[R1][D] = whitePieces[QUEEN];
		board[R1][E] = whitePieces[KING];
		board[R1][F] = whitePieces[BISHOP2];
		board[R1][G] = whitePieces[KNIGHT2];
		board[R1][H] = whitePieces[ROOK2];
	}

	void doMove()
	{
	}

	void showPosition() // Debug tulostus
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
