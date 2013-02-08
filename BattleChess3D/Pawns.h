#include "ConsoleColor.h"
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

	virtual void debugPrint()=0;
};

class King: public Piece
{
public:
	King(Owner o)
	{
		owner = o;
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
	Queen(Owner o)
	{
		owner = o;
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
	Rook(Owner o)
	{
		owner = o;
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
	Bishop(Owner o)
	{
		owner = o;
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
	Knight(Owner o)
	{
		owner = o;
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
	Pawn(Owner o)
	{
		owner = o;
	}
	~Pawn(){}

	void debugPrint()
	{
		cout << " P ";
	}
};
