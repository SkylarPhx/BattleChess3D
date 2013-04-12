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

	Who whoIsOn(short col, short row)
	{
		return board[row][col]->who;
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

private:
	// For checking any square.
	// Returns true if a piece is found.
	bool moveCheck(Piece* p, list<Move> &m, Owner o, short fC, short fR, short tC, short tR)
	{
		if(p == NULL)
		{
			// Legal move!
			m.emplace_back(fC, fR, tC, tR);
			// Continue checking for moves.
			return false;
		}
		if(p->owner != o)
		{
			// Enemy sighted!
			// Legal move!
			m.emplace_back(fC, fR, tC, tR);
			// No more moves!
		}
		return true;
	}

	bool moveCheckPawn(Piece* p, list<Move> &m, short fC, short fR, short tC, short tR)
	{
		if(p != NULL) return false;	
		m.emplace_back(fC, fR, tC, tR);
		return true;
	}

	void eatCheckPawn(Piece* p, list<Move> &m, Owner o, short fC, short fR, short tC, short tR)
	{
		if(p != NULL && p->owner != o)
		{
			m.emplace_back(fC, fR, tC, tR);
		}
	}

	bool threatenCheck(Piece* p, Owner o, short &threats, Who secondPieceType, Piece* &t)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return false;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == o) return true;
		// Is it queen or rook/bishop = is the king threatened?
		if(p->who == QUEEN || p->who == secondPieceType)
		{
			cout << "Found a threat at";
			printColRow(p->col, p->row);
			cout << endl;
			t = p;
			threats++;
		}
		// Stop checking.
		return true;
	}

	void threatenCheck1Piece(Piece* p, Owner o, short &threats, Who pieceType, Piece* &t)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == o) return;
		// Is it knight or pawn = is the king threatened?
		if(p->who == pieceType)
		{
			cout << "Found a threat at";
			printColRow(p->col, p->row);
			cout << endl;
			t = p;
			threats++;
		}
	}

	bool canMoveOwn(Piece* p, Owner o, list<Move> &m, Who secondPieceType, short tC, short tR)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return false;
		// Vihollisen nappi. Lopeta tarkistus.
		if(p->owner != o) return true;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->who == QUEEN || p->who == secondPieceType)
		{
			m.emplace_back(p->col, p->row, tC, tR);
		}
		return true;
	}

	void canMoveOwn1Piece(Piece* p, Owner o, list<Move> &m, Who pieceType, short tC, short tR)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->owner == o && p->who == pieceType)
		{
			m.emplace_back(p->col, p->row, tC, tR);
		}
	}

	bool canMoveOwnPawn(Piece* p, Owner o, list<Move> &m, short tC, short tR)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return true;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->owner == o && p->who == PAWN)
		{
			m.emplace_back(p->col, p->row, tC, tR);
		}
		return false;
	}

	bool isKingThreatened(Piece &king, Piece* &threatener)
	{
		// Uhkien määrä (tarvitaan myöhemmin)
		short threats = 0;

		// Up/Down/Left/Right: queens, rooks
		// Up
		for(short r = king.row + 1; r < 8; r++)
		{
			if(threatenCheck(board[r][king.col], king.owner, threats, ROOK, threatener))
			{
				break;
			}
		}
		// Right
		for(short c = king.col + 1; c < 8; c++)
		{
			if(threatenCheck(board[king.row][c], king.owner, threats, ROOK, threatener))
			{
				break;
			}
		}
		// Down
		for(short r = king.row - 1; r >= 0; r--)
		{
			if(threatenCheck(board[r][king.col], king.owner, threats, ROOK, threatener))
			{
				break;
			}
		}
		// Left
		for(short c = king.col - 1; c >= 0; c--)
		{
			if(threatenCheck(board[king.row][c], king.owner, threats, ROOK, threatener))
			{
				break;
			}
		}

		// Sideways: queens, bishops
		// Up-right
		for(short r = king.row + 1, c = king.col + 1; r < 8 && c < 8; r++, c++)
		{
			if(threatenCheck(board[r][c], king.owner, threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Right-down
		for(short r = king.row - 1, c = king.col + 1; r >= 0 && c < 8; r--, c++)
		{
			if(threatenCheck(board[r][c], king.owner, threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Down-left
		for(short r = king.row - 1, c = king.col - 1; r >= 0 && c >= 0; r--, c--)
		{
			if(threatenCheck(board[r][c], king.owner, threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Left-up
		for(short r = king.row + 1, c = king.col - 1; r < 8 && c >= 0; r++, c--)
		{
			if(threatenCheck(board[r][c], king.owner, threats, BISHOP, threatener))
			{
				break;
			}
		}

		// Checking for knights
		short r = king.row, c = king.col;
		// Up
		if(r < 6)
		{
			// Right
			if(c < 7)
			threatenCheck1Piece(board[r + 2][c + 1], king.owner, threats, KNIGHT, threatener);
			// Left
			if(c > 0)
			threatenCheck1Piece(board[r + 2][c - 1], king.owner, threats, KNIGHT, threatener);
		}
		// Right
		if(c < 6)
		{
			// Up
			if(r < 7)
			threatenCheck1Piece(board[r + 1][c + 2], king.owner, threats, KNIGHT, threatener);
			// Down
			if(r > 0)
			threatenCheck1Piece(board[r - 1][c + 2], king.owner, threats, KNIGHT, threatener);
		}
		// Down
		if(r > 1)
		{
			// Right
			if(c < 7)
			threatenCheck1Piece(board[r - 2][c + 1], king.owner, threats, KNIGHT, threatener);
			// Left
			if(c > 0)
			threatenCheck1Piece(board[r - 2][c - 1], king.owner, threats, KNIGHT, threatener);
		}
		// Left
		if(c > 1)
		{
			// Up
			if(r < 7)
			threatenCheck1Piece(board[r + 1][c - 2], king.owner, threats, KNIGHT, threatener);
			// Down
			if(r > 0)
			threatenCheck1Piece(board[r - 1][c - 2], king.owner, threats, KNIGHT, threatener);
		}

		// Checking for pawns.
		r = king.row;
		if(whoseTurn == WHITE)
		{
			// No threat from pawns.
			if(r == 7) goto SkipPawns;
			r++;
		}
		else
		{
			// No threat from pawns.
			if(r == 0) goto SkipPawns;
			r--;
		}
		c = king.col;
		// Right side
		if(c < 7)
		threatenCheck1Piece(board[r][c + 1], king.owner, threats, PAWN, threatener);
		// Left side
		if(c > 0)
		threatenCheck1Piece(board[r][c - 1], king.owner, threats, PAWN, threatener);
		SkipPawns:

		// Useita uhkaajia.
		// Siispä ei ole mahdollista laittaa eteen mitään.
		if(threats > 1) threatener = NULL;
		return threats;
	}

	void checkOwnMovesToHere(list<Move> &m, Owner o, short tC, short tR)
	{
		// Samanlainen tarkistus kuin uhkaamisista.
		// Tässä vaan tarkistetaan toisin päin.

		// Up/Down/Left/Right: queens, rooks
		// Up
		for(short r = tR + 1; r < 8; r++)
		{
			if(canMoveOwn(board[r][tC], o, m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Right
		for(short c = tC + 1; c < 8; c++)
		{
			if(canMoveOwn(board[tR][c], o, m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Down
		for(short r = tR - 1; r >= 0; r--)
		{
			if(canMoveOwn(board[r][tC], o, m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Left
		for(short c = tC - 1; c >= 0; c--)
		{
			if(canMoveOwn(board[tR][c], o, m, ROOK, tC, tR))
			{
				break;
			}
		}

		// Sideways: queens, bishops
		// Up-right
		for(short r = tR + 1, c = tC + 1; r < 8 && c < 8; r++, c++)
		{
			if(canMoveOwn(board[r][c], o, m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Right-down
		for(short r = tR - 1, c = tC + 1; r >= 0 && c < 8; r--, c++)
		{
			if(canMoveOwn(board[r][c], o, m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Down-left
		for(short r = tR - 1, c = tC - 1; r >= 0 && c >= 0; r--, c--)
		{
			if(canMoveOwn(board[r][c], o, m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Left-up
		for(short r = tR + 1, c = tC - 1; r < 8 && c >= 0; r++, c--)
		{
			if(canMoveOwn(board[r][c], o, m, BISHOP, tC, tR))
			{
				break;
			}
		}

		// Checking for knights
		short r = tR, c = tC;
		// Up
		if(r < 6)
		{
			// Right
			if(c < 7)
			canMoveOwn1Piece(board[r + 2][c + 1], o, m, KNIGHT, tC, tR);
			// Left
			if(c > 0)
			canMoveOwn1Piece(board[r + 2][c - 1], o, m, KNIGHT, tC, tR);
		}
		// Right
		if(c < 6)
		{
			// Up
			if(r < 7)
			canMoveOwn1Piece(board[r + 1][c + 2], o, m, KNIGHT, tC, tR);
			// Down
			if(r > 0)
			canMoveOwn1Piece(board[r - 1][c + 2], o, m, KNIGHT, tC, tR);
		}
		// Down
		if(r > 1)
		{
			// Right
			if(c < 7)
			canMoveOwn1Piece(board[r - 2][c + 1], o, m, KNIGHT, tC, tR);
			// Left
			if(c > 0)
			canMoveOwn1Piece(board[r - 2][c - 1], o, m, KNIGHT, tC, tR);
		}
		// Left
		if(c > 1)
		{
			// Up
			if(r < 7)
			canMoveOwn1Piece(board[r + 1][c - 2], o, m, KNIGHT, tC, tR);
			// Down
			if(r > 0)
			canMoveOwn1Piece(board[r - 1][c - 2], o, m, KNIGHT, tC, tR);
		}

		// Checking for pawns.
		r = tR;
		if(whoseTurn == BLACK)
		{
			if(r == 7) return;
			r++;
		}
		else
		{
			if(r == 0) return;
			r--;
		}
		c = tC;
		// Can move pawn to eat.
		if(board[tR][tC] != NULL)
		{
			// Right side
			if(c < 7)
			canMoveOwn1Piece(board[r][c + 1], o, m, PAWN, tC, tR);
			// Left side
			if(c > 0)
			canMoveOwn1Piece(board[r][c - 1], o, m, PAWN, tC, tR);
		}
		else
		{
			// Can only move pawn.
			if(canMoveOwnPawn(board[r][c], o, m, tC, tR))
			{
				if(whoseTurn == WHITE)
				{
					if(r == 2)
					canMoveOwnPawn(board[r - 1][c], o, m, tC, tR);
				}
				else
				{
					if(r == 5)
					canMoveOwnPawn(board[r + 1][c], o, m, tC, tR);
				}
			}
		}
	}

public:
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
			cout << redWhite << "\n*** White's Possible Moves ***" << greyBlack << endl;
			playersPieces = &whitePieces;
			enemysPieces = &blackPieces;
		}
		else
		{
			cout << redBlack << "\n*** Black's Possible Moves ***" << greyBlack << endl;
			playersPieces = &blackPieces;
			enemysPieces = &whitePieces;
		}

		// Kuninkaan käsittely tähän
		auto king = (*playersPieces).begin();
		Piece *threatener = NULL;

		// Uhataanko?
		if(isKingThreatened(*king, threatener))
		{
			cout << "Check!" << endl;

			// Voiko kuningas liikkua?
			// OMA FUNKTIO TÄLLE!

			// Jos vain yksi vihollinen uhkaa, tarkista voiko laittaa eteen nappeja.
			// Tämä on totta vain jos kuningasta uhataan.
			if(threatener != NULL)
			{
				// Tarkista pelkästään voiko tähän väliin laittaa nappeja.
				// TEE OMA FUNKTIO JOSSA TARKISTETAAN VOIKO OMAN LAITTAA RUUTUUN!
				// Vihollistyypin perusteella katsotaan mitä ruutuja tarkistetaan!
				short row = threatener->row, col = threatener->col;
				switch(threatener->who)
				{
				case QUEEN:
				case ROOK:
				case BISHOP:
					{
						// Tarkistetaan suora jono nappia kohti nappi mukaanlukien.
						for(short r = row, c = col; r != king->row || c != king->col;)
						{
							// Kutsu funktiota joka tarkistaa voiko oman napin siirtää tähän kohtaan laudalla.
							checkOwnMovesToHere(moves, whoseTurn, c, r);

							if(r > king->row) r--;
							else if(r < king->row) r++;

							if(c > king->col) c--;
							else if(c < king->col) c++;
						}
					}
					break;
				case KNIGHT:
					// Voiko hevosen syödä?
					// Kutsu funktiota joka tarkistaa voiko oman napin siirtää tähän kohtaan laudalla.
					checkOwnMovesToHere(moves, whoseTurn, col, row);
					break;
				case PAWN:
					// Voiko sotilaan syödä?
					// Kutsu funktiota joka tarkistaa voiko oman napin siirtää tähän kohtaan laudalla.
					checkOwnMovesToHere(moves, whoseTurn, col, row);
					break;
				}
			}

			// Muita nappeja ei voi siirtää muualle!
			return moves.size();
		}

		// Voiko kuningas liikkua?
		// OMA FUNKTIO TÄLLE!

		// Sitten loput napit
		for(auto p = (*playersPieces).begin()++; p != (*playersPieces).end(); p++)
		{
			switch(p->who)
			{
			case QUEEN:
				{
					// First up
					for(short r = p->row + 1; r < 8; r++)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then up-right
					for(short r = p->row + 1, c = p->col + 1; r < 8 && c < 8; r++, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then right
					for(short c = p->col + 1; c <= 7; c++)
					{
						if(moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then right-down
					for(short r = p->row - 1, c = p->col + 1; r >= 0 && c <8; r--, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then down-left
					for(short r = p->row - 1, c = p->col - 1; r >= 0 && c >= 0; r--, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then left
					for(short c = p->col - 1; c >= 0; c--)
					{
						if(moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}
				}
				break;
			case ROOK:
				{
					// First up
					for(short r = p->row + 1; r < 8; r++)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then right
					for(short c = p->col + 1; c <= 7; c++)
					{
						if(moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, whoseTurn, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then left
					for(short c = p->col - 1; c >= 0; c--)
					{
						if(moveCheck(board[p->row][c], moves, whoseTurn, p->col, p->row, c, p->row))
						{
							break;
						}
					}
				}
				break;
			case BISHOP:
				{
					// Then up-right
					for(short r = p->row + 1, c = p->col + 1; r < 8 && c < 8; r++, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then right-down
					for(short r = p->row - 1, c = p->col + 1; r >= 0 && c < 8; r--, c++)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then down-left
					for(short r = p->row - 1, c = p->col - 1; r >= 0 && c >= 0; r--, c--)
					{
						if(moveCheck(board[r][c], moves, whoseTurn, p->col, p->row, c, r))
						{
							break;
						}
					}
				}
				break;
			case KNIGHT:
				{
					short r = p->row, c = p->col;
					// Up
					if(r < 6)
					{
						// Right
						if(c < 7)
						moveCheck(board[r + 2][c + 1], moves, whoseTurn, c, r, c + 1, r + 2);
						// Left
						if(c > 0)
						moveCheck(board[r + 2][c - 1], moves, whoseTurn, c, r, c - 1, r + 2);
					}
					// Right
					if(c < 6)
					{
						// Up
						if(r < 7)
						moveCheck(board[r + 1][c + 2], moves, whoseTurn, c, r, c + 2, r + 1);
						// Down
						if(r > 0)
						moveCheck(board[r - 1][c + 2], moves, whoseTurn, c, r, c + 2, r - 1);
					}
					// Down
					if(r > 1)
					{
						// Right
						if(c < 7)
						moveCheck(board[r - 2][c + 1], moves, whoseTurn, c, r, c + 1, r - 2);
						// Left
						if(c > 0)
						moveCheck(board[r - 2][c - 1], moves, whoseTurn, c, r, c - 1, r - 2);
					}
					// Left
					if(c > 1)
					{
						// Up
						if(r < 7)
						moveCheck(board[r + 1][c - 2], moves, whoseTurn, c, r, c - 2, r + 1);
						// Down
						if(r > 0)
						moveCheck(board[r - 1][c - 2], moves, whoseTurn, c, r, c - 2, r - 1);
					}
				}
				break;
			case PAWN:
				{
					short r = p->row, c = p->col;
					if(whoseTurn == WHITE)
					{
						if(r < 7) r++;
					}
					else
					{
						if(r > 0) r--;
					}
					// Right side
					if(c < 7)
					eatCheckPawn(board[r][c + 1], moves, whoseTurn, c, p->row, c + 1, r);
					// Left side
					if(c > 0)
					eatCheckPawn(board[r][c - 1], moves, whoseTurn, c, p->row, c - 1, r);
					// Move forward
					if(moveCheckPawn(board[r][c], moves, c, p->row, c, r))
					{
						if(whoseTurn == WHITE)
						{
							if(r == 2)
							moveCheckPawn(board[r + 1][c], moves, c, p->row, c, r + 1);
						}
						else
						{
							if(r == 5)
							moveCheckPawn(board[r - 1][c], moves, c, p->row, c, r - 1);
						}
					}
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
