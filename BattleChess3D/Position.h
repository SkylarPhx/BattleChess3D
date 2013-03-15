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
		fromCol = command[0] - 97;
		fromRow = command[1] - 49;
		toCol = command[3] - 97;
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
		cout << " " << (char)(col + 97) << (row + 1);
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

	bool threathenCheck(Piece* &p, Owner o, bool &threat, Who secondPieceType)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return false;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == o) return true;
		// Is it queen or rook/bishop = is the king threathened?
		if(p->who == QUEEN || p->who == secondPieceType) threat = true;
		// Stop checking.
		return true;
	}
	
	bool threathenCheck1Piece(Piece* &p, Owner o, bool &threat, Who pieceType)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return false;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == o) return true;
		// Is it queen or rook/bishop = is the king threathened?
		if(p->who == PAWN) threat = true;
		// Stop checking.
		return true;
	}

	short generateLegalMoves(list<Move> &moves)
	{
		// • Käydään läpi vuorossa olevan pelaajan nappulat.
		// • Ensimmäisenä on aina kuningas.
		// • Tarkistetaan uhkaako vihollisen nappulat kuningasta.
		// • Jos yli 2 nappulaa uhkaa, siirrytään selaamaan kuninkaan siirtoja.
		// • Jos 1 nappula uhkaa, katsotaan voiko oman nappulan laittaa väliin.
		// • Jos kuningasta voi siirtää, jatketaan muihin omiin nappuloihin.
		// • Niissä ei enää pohdita kuninkaan uhkaamisia.

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

		// Kuninkaan käsittely tähän
		auto king = (*playersPieces).begin();

		// Uhataanko?
		bool isThreathened = false;

		// Up/Down/Left/Right: queens, rooks
		// Up
		for(short r = king->row + 1; r < 8; r++)
		{
			if(threathenCheck(board[r][king->col], king->owner, isThreathened, ROOK))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Right
		for(short c = king->col + 1; c < 8; c++)
		{
			if(threathenCheck(board[king->row][c], king->owner, isThreathened, ROOK))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Down
		for(short r = king->row - 1; r >= 0; r--)
		{
			if(threathenCheck(board[r][king->col], king->owner, isThreathened, ROOK))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Left
		for(short c = king->col - 1; c >= 0; c--)
		{
			if(threathenCheck(board[king->row][c], king->owner, isThreathened, ROOK))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}

		// Sideways: queens, bishops
		// Up-right
		for(short r = king->row + 1, c = king->col + 1; r < 8 && c < 8; r++, c++)
		{
			if(threathenCheck(board[r][c], king->owner, isThreathened, BISHOP))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Right-down
		for(short r = king->row - 1, c = king->col + 1; r >= 0 && c < 8; r--, c++)
		{
			if(threathenCheck(board[r][c], king->owner, isThreathened, BISHOP))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Down-left
		for(short r = king->row - 1, c = king->col - 1; r >= 0 && c >= 0; r--, c--)
		{
			if(threathenCheck(board[r][c], king->owner, isThreathened, BISHOP))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Left-up
		for(short r = king->row + 1, c = king->col - 1; r < 8 && c >= 0; r++, c--)
		{
			if(threathenCheck(board[r][c], king->owner, isThreathened, BISHOP))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}

		// Checking for pawns.
		short r = king->row;
		if(whoseTurn == WHITE)
		{
			// No threat from pawns.
			if(r == 7) goto CheckKingsMoves;
			r++;
		}
		else
		{
			// No threat from pawns.
			if(r == 0) goto CheckKingsMoves;
			r--;
		}
		short c = king->col;
		// Right side
		if(c < 7)
		if(threathenCheck1Piece(board[r][c], king->owner, isThreathened, PAWN))
		{
			if(isThreathened) goto CheckKingsMoves;
			// No need for break, because of goto.
		}
		// Left side
		if(c > 0)
		if(threathenCheck1Piece(board[r][c], king->owner, isThreathened, PAWN))
		{
			if(isThreathened) goto CheckKingsMoves;
			// No need for break, because of goto.
		}

		// Checking for knights
		r = king->row;
		c = king->col;
		// Up
		if(r < 6)
		{
			// Right
			if(c < 7)
			if(threathenCheck1Piece(board[r + 2][c + 1], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
			// Left
			if(c > 0)
			if(threathenCheck1Piece(board[r + 2][c - 1], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Right
		if(c < 6)
		{
			// Up
			if(r < 7)
			if(threathenCheck1Piece(board[r + 1][c + 2], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
			// Down
			if(r > 0)
			if(threathenCheck1Piece(board[r - 1][c + 2], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Down
		if(r > 1)
		{
			// Right
			if(c < 7)
			if(threathenCheck1Piece(board[r - 2][c + 1], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
			// Left
			if(c > 0)
			if(threathenCheck1Piece(board[r - 2][c - 1], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
		// Left
		if(c > 1)
		{
			// Up
			if(r < 7)
			if(threathenCheck1Piece(board[r + 1][c - 2], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
			// Down
			if(r > 0)
			if(threathenCheck1Piece(board[r - 1][c - 2], king->owner, isThreathened, KNIGHT))
			{
				if(isThreathened) goto CheckKingsMoves;
				// No need for break, because of goto.
			}
		}
	
		// Jos ei niin voiko liikkua?
CheckKingsMoves:
		if(isThreathened) cout << "Check!" << endl;
		

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
					for (short c = p->col + 1; c <= 7; c++) {
						if (moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row)) break;
					}
					cout << endl;

					// Then right-down
					for (short r = p->row - 1, c = p -> col + 1; r >= 0 && c <8; r--, c++) {
						if (moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r)) break;
					}
					cout << endl;

					// Then down-left
					for (short r = p->row - 1, c = p -> col - 1; r >= 0 && c >= 0; r--, c--) {
						if (moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then left
					for (short c = p->col - 1; c >= 0; c--) {
						if (moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row)) break;
					}
					cout << endl;

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;
				}
				break;
			case ROOK:
				{
					// Some debug info.
					cout << "Moves of Rook in";
					printColRow(p->col, p->row);
					cout << endl;

					// First up
					for(short r = p->row + 1; r < 8; r++)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r)) break;
					}
					cout << endl;

					// Then right
					for (short c = p->col + 1; c <= 7; c++) {
						if (moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row)) break;
					}
					cout << endl;

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r)) break;
					}
					cout << endl;

					// Then left
					for (short c = p->col - 1; c >= 0; c--) {
						if (moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row)) break;
					}
					cout << endl;
				}
				break;
			case BISHOP:
				{
					// Some debug info.
					cout << "Moves of Bishop in";
					printColRow(p->col, p->row);
					cout << endl;

					// Then up-right
					for(short r = p->row + 1, c = p->col + 1; r < 8 && c < 8; r++, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then right-down
					for (short r = p->row - 1, c = p -> col + 1; r >= 0 && c <8; r--, c++) {
						if (moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;

					// Then down-left
					for (short r = p->row - 1, c = p -> col - 1; r >= 0 && c >= 0; r--, c--) {
						if (moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r)) break;
					}
					cout << endl;
				}
				break;
			case KNIGHT:
				{
					// Some debug info.
					cout << "Moves of Knight in";
					printColRow(p->col, p->row);
					cout << endl;

					// up then left
					//for (short r = p->row + 2, c = p->col - 1; r < 8 && c >= 0;

				}
				break;
			case PAWN:
				{
			//		// Some debug info.
			//		cout << "Moves of Pawn in";
			//		printColRow(p->col, p->row);
			//		cout << endl;
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
		cout << "\n    a  b  c  d  e  f  g  h" << endl;
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
				cout << " ";
				p->debugPrint();
				cout << " ";
			}
			turn = !turn;
			cout << greyBlack << " " << (i + 1) << endl;
		}
		cout << "    a  b  c  d  e  f  g  h" << endl;

		cout << "\n White" << endl;
		for(auto &p: whitePieces)
		{
			p.debugPrint();
			cout << (char)(p.col + 97) << (p.row + 1) << " ";
		}
		
		cout << "\n\n Black" << endl;
		for(auto &p: blackPieces)
		{
			p.debugPrint();
			cout << (char)(p.col + 97) << (p.row + 1) << " ";
		}
		cout << endl;
	}
};
