#include "stdafx.h"
using namespace std;

enum Owner
{
	BLACK, WHITE
};

enum Who
{
	KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN
};

enum File
{
	A, B, C, D, E, F, G, H
};

enum Rank
{
	R1, R2, R3, R4, R5, R6, R7, R8
};

class Piece
{
public:
	Who who;
	Owner owner;
	// Location on the chess board.
	short row, col;

	Piece(Owner o, Who w, Rank r, File c)
	{
		owner = o;
		who = w;
		row = r;
		col = c;
	}
	~Piece(){}

	void debugPrint()
	{
		switch(who)
		{
		case KING:
			cout << " K ";
			break;
		case QUEEN:
			cout << " Q ";
			break;
		case ROOK:
			cout << " R ";
			break;
		case BISHOP:
			cout << " B ";
			break;
		case KNIGHT:
			cout << " N ";
			break;
		case PAWN:
			cout << " P ";
			break;
		}
	};
};
