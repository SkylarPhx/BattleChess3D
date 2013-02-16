#include "stdafx.h"
using namespace std;

enum Owner
{
	BLACK, WHITE
};

class Piece
{
public:
	Owner owner;
	// Location on the chess board.
	short row, col;

	virtual void debugPrint()=0;
};

class King: public Piece
{
public:
	King(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~King(){}

	void debugPrint()
	{
		cout << " K ";
	}
};

class Queen: public Piece
{
public:
	Queen(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~Queen(){}

	void debugPrint()
	{
		cout << " Q ";
	}
};

class Rook: public Piece
{
public:
	Rook(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~Rook(){}

	void debugPrint()
	{
		cout << " R ";
	}
};

class Bishop: public Piece
{
public:
	Bishop(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~Bishop(){}

	void debugPrint()
	{
		cout << " B ";
	}
};

class Knight: public Piece
{
public:
	Knight(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~Knight(){}

	void debugPrint()
	{
		cout << " N ";
	}
};

class Pawn: public Piece
{
public:
	Pawn(Owner o, short r, short c)
	{
		owner = o;
		row = r;
		col = c;
	}
	~Pawn(){}

	void debugPrint()
	{
		cout << " P ";
	}
};
