#include "stdafx.h"
#include "Pieces.h"
#include "ConsoleColor.h"
using namespace std;

class Move
{
public:
	short fromRow, fromCol, toRow, toCol;
	bool isCastling;

	Move(){}
	Move(short fC, short fR, short tC, short tR)
	{
		fromCol = fC;
		fromRow = fR;
		toCol = tC;
		toRow = tR;
	}
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
	// Keeps track of turns.
	Owner whoseTurn;

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

		whoseTurn = WHITE;
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

		whoseTurn = p.whoseTurn;
	}
	
	void printColRow(short col, short row)
	{
		cout << " " << (char)(col + 65) << (row + 1);
	}

	// For checking any square.
	// Returns true if a piece is found.
	bool moveCheck(Piece* &p, list<Move> &m, Owner o, short fC, short fR, short tC, short tR)
	{
		if(p == NULL)
		{
			// Legal move!
			printColRow(tC, tR);
			m.emplace_back(fC, fR, tC, tR);
			// Continue checking for moves.
			return false;
		}
		else if(p->owner != o)
		{
			// Enemy sighted!
			// Legal move!
			printColRow(tC, tR);
			m.emplace_back(fC, fR, tC, tR);
			// No more moves!
		}
		return true;
	}

	short generateLegalMoves(list<Move> &moves)
	{
		// ï K‰yd‰‰n l‰pi vuorossa olevan pelaajan nappulat.
		// ï Ensimm‰isen‰ on aina kuningas.
		// ï Tarkistetaan uhkaako vihollisen nappulat kuningasta.
		// ï Jos yli 2 nappulaa uhkaa, siirryt‰‰n selaamaan kuninkaan siirtoja.
		// ï Jos 1 nappula uhkaa, katsotaan voiko oman nappulan laittaa v‰liin.
		// ï Jos kuningasta voi siirt‰‰, jatketaan muihin omiin nappuloihin.
		// ï Niiss‰ ei en‰‰ pohdita kuninkaan uhkaamisia.

		list<Piece> *playersPieces, *enemysPieces;
		if(whoseTurn == WHITE)
		{
			playersPieces = &whitePieces;
			enemysPieces = &blackPieces;
		}
		else
		{
			playersPieces = &blackPieces;
			enemysPieces = &whitePieces;
		}

		// Kuninkaan k‰sittely t‰h‰n

		// Sitten loput napit
		for(auto p = (*playersPieces).begin()++; p != (*playersPieces).end(); p++)
		{
			switch(p->who)
			{
			case QUEEN:
				{
					// Some debug info.
					cout << "Moves of Queen in";
					printColRow(p->col, p->row);
					cout << endl;

					// First up
					for(short r = p->row + 1; r < 8; r++)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r)) break;
					}
					cout << endl;

					// Then up-right
					for(short r = p->row + 1, c = p->col + 1; r < 8 && c < 8; r++, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then right

					// Then right-down

					// Then down

					// Then down-left

					// Then left

					// Then left-up

				}
				break;
			case ROOK:
				{
					// Some debug info.
					cout << "Moves of Rook in";
					printColRow(p->col, p->row);
					cout << endl;

				}
				break;
			case BISHOP:
				{
					// Some debug info.
					cout << "Moves of Bishop in";
					printColRow(p->col, p->row);
					cout << endl;

				}
				break;
			case KNIGHT:
				{
					// Some debug info.
					cout << "Moves of Knight in";
					printColRow(p->col, p->row);
					cout << endl;

				}
				break;
			case PAWN:
				{
					// Some debug info.
					cout << "Moves of Pawn in";
					printColRow(p->col, p->row);
					cout << endl;

				}
				break;
			}
		}

		return moves.size();
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

	void changeTurn()
	{
		// Turn changes.
		whoseTurn = (whoseTurn == WHITE) ? BLACK : WHITE;
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
