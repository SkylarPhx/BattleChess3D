#include "stdafx.h"
#include "Pieces.h"
#include "ConsoleColor.h"
using namespace std;

class Move
{
public:
	short fromRow, fromCol, toRow, toCol;
	// 1 = Kingside castling
	// 2 = Queenside castling
	// 3 = Double pawn move
	// 4 = En Passant move
	short special;
	bool AI;

	Move(){}
	Move(short fC, short fR, short tC, short tR, short type = 0)
	{
		fromCol = fC;
		fromRow = fR;
		toCol = tC;
		toRow = tR;
		special = type;
		AI = true;
	}
	~Move(){}

	void setMove(string &command)
	{
		fromCol = command[0] - 97;
		fromRow = command[1] - 49;
		toCol = command[3] - 97;
		toRow = command[4] - 49;
		special = 0;
		AI = false;
	}
	void setSpecial(char c)
	{
		special = c - 48;
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
	// bit 1 = White's kingside
	// bit 2 = White's queenside
	// bit 3 = Black's kingside
	// bit 4 = Black's queenside
	short canCastle;
	// For en passant move.
	Piece *passer;

public:
	Position(){}
	Position(Position &p)
	{
		copyPosition(p);
	}

	~Position(){}

	void showSpecialInfo()
	{
		cout << "Castling bits: " << canCastle << endl;
		cout << "En Passer: " << passer << endl;
	}

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
		canCastle = 15;
		passer = NULL;
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
		canCastle = p.canCastle;
		passer = p.passer;
	}

	void printColRow(short col, short row)
	{
		cout << " " << (char)(col + 97) << (row + 1);
	}

private:
	// For checking any square.
	// Returns true if a piece is found.
	bool moveCheck(Piece* p, list<Move> &m, short fC, short fR, short tC, short tR)
	{
		if(p == NULL)
		{
			// Legal move!
			m.emplace_back(fC, fR, tC, tR);
			// Continue checking for moves.
			return false;
		}
		if(p->owner != whoseTurn)
		{
			// Enemy sighted!
			// Legal move!
			m.emplace_back(fC, fR, tC, tR);
			// No more moves!
		}
		return true;
	}

	bool moveCheckPawn(Piece* p, list<Move> &m, short fC, short fR, short tC, short tR, short two = 0)
	{
		if(p != NULL) return false;
		m.emplace_back(fC, fR, tC, tR, two);
		return true;
	}

	void eatCheckPawn(Piece* p, list<Move> &m, short fC, short fR)
	{
		if(p != NULL && p->owner != whoseTurn)
		{
			m.emplace_back(fC, fR, p->col, p->row);
		}
	}

	void enPassantCheckPawn(Piece* p, list<Move> &m, short fC, short tR)
	{
		if(p == passer)
		{
			m.emplace_back(fC, p->row, p->col, tR, 4);
		}
	}

	bool threatenCheck(Piece* p, short &threats, Who secondPieceType, Piece* &t)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return false;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == whoseTurn) return true;
		// Is it queen or rook/bishop = is the king threatened?
		if(p->who == QUEEN || p->who == secondPieceType)
		{
			t = p;
			threats++;
		}
		// Stop checking.
		return true;
	}

	void threatenCheck1Piece(Piece* p, short &threats, Who pieceType, Piece* &t)
	{
		// No piece, no threat.
		// Continue checking.
		if(p == NULL) return;
		// Our piece, no threat.
		// Stop checking.
		if(p->owner == whoseTurn) return;
		// Is it knight or pawn = is the king threatened?
		if(p->who == pieceType)
		{
			t = p;
			threats++;
		}
	}

	bool canMoveOwn(Piece* p, list<Move> &m, Who secondPieceType, short tC, short tR)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return false;
		// Vihollisen nappi. Lopeta tarkistus.
		if(p->owner != whoseTurn) return true;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->who == QUEEN || p->who == secondPieceType)
		{
			m.emplace_back(p->col, p->row, tC, tR);
		}
		return true;
	}

	void canMoveOwn1Piece(Piece* p, list<Move> &m, Who pieceType, short tC, short tR)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->owner == whoseTurn && p->who == pieceType)
		{
			m.emplace_back(p->col, p->row, tC, tR);
		}
	}

	void canMoveEnPassant(Piece* p, list<Move> &m, short tR, short direction)
	{
		if(p == passer)
		{
			canMoveOwnPawn(board[p->row][p->col + direction], m, p->col, tR, 4);
		}
	}

	bool canMoveOwnPawn(Piece* p, list<Move> &m, short tC, short tR, short two = 0)
	{
		// Ei nappia. Tästä ei voi siirtää kuninkaan suojeluun.
		if(p == NULL) return true;
		// Oma oikeantyyppinen nappi siirrettävissä kuninkaan suojeluun.
		if(p->owner == whoseTurn && p->who == PAWN)
		{
			m.emplace_back(p->col, p->row, tC, tR, two);
		}
		return false;
	}

	bool isKingThreatened(short col, short row, Piece* &threatener)
	{
		// Uhkien määrä (tarvitaan myöhemmin)
		short threats = 0;

		// Up/Down/Left/Right: queens, rooks
		// Up
		for(short r = row + 1; r < 8; r++)
		{
			if(threatenCheck(board[r][col], threats, ROOK, threatener))
			{
				break;
			}
		}
		// Right
		for(short c = col + 1; c < 8; c++)
		{
			if(threatenCheck(board[row][c], threats, ROOK, threatener))
			{
				break;
			}
		}
		// Down
		for(short r = row - 1; r >= 0; r--)
		{
			if(threatenCheck(board[r][col], threats, ROOK, threatener))
			{
				break;
			}
		}
		// Left
		for(short c = col - 1; c >= 0; c--)
		{
			if(threatenCheck(board[row][c], threats, ROOK, threatener))
			{
				break;
			}
		}

		// Sideways: queens, bishops
		// Up-right
		for(short r = row + 1, c = col + 1; r < 8 && c < 8; r++, c++)
		{
			if(threatenCheck(board[r][c], threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Right-down
		for(short r = row - 1, c = col + 1; r >= 0 && c < 8; r--, c++)
		{
			if(threatenCheck(board[r][c], threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Down-left
		for(short r = row - 1, c = col - 1; r >= 0 && c >= 0; r--, c--)
		{
			if(threatenCheck(board[r][c], threats, BISHOP, threatener))
			{
				break;
			}
		}
		// Left-up
		for(short r = row + 1, c = col - 1; r < 8 && c >= 0; r++, c--)
		{
			if(threatenCheck(board[r][c], threats, BISHOP, threatener))
			{
				break;
			}
		}

		// Checking for knights
		short r = row, c = col;
		// Up
		if(r < 6)
		{
			// Right
			if(c < 7)
			threatenCheck1Piece(board[r + 2][c + 1], threats, KNIGHT, threatener);
			// Left
			if(c > 0)
			threatenCheck1Piece(board[r + 2][c - 1], threats, KNIGHT, threatener);
		}
		// Right
		if(c < 6)
		{
			// Up
			if(r < 7)
			threatenCheck1Piece(board[r + 1][c + 2], threats, KNIGHT, threatener);
			// Down
			if(r > 0)
			threatenCheck1Piece(board[r - 1][c + 2], threats, KNIGHT, threatener);
		}
		// Down
		if(r > 1)
		{
			// Right
			if(c < 7)
			threatenCheck1Piece(board[r - 2][c + 1], threats, KNIGHT, threatener);
			// Left
			if(c > 0)
			threatenCheck1Piece(board[r - 2][c - 1], threats, KNIGHT, threatener);
		}
		// Left
		if(c > 1)
		{
			// Up
			if(r < 7)
			threatenCheck1Piece(board[r + 1][c - 2], threats, KNIGHT, threatener);
			// Down
			if(r > 0)
			threatenCheck1Piece(board[r - 1][c - 2], threats, KNIGHT, threatener);
		}

		// Checking for pawns.
		if(whoseTurn == WHITE)
		{
			// No threat from pawns.
			if(row == 7) goto SkipPawns;
			row++;
		}
		else
		{
			// No threat from pawns.
			if(row == 0) goto SkipPawns;
			row--;
		}
		// Right side
		if(col < 7)
		threatenCheck1Piece(board[row][col + 1], threats, PAWN, threatener);
		// Left side
		if(col > 0)
		threatenCheck1Piece(board[row][col - 1], threats, PAWN, threatener);
		SkipPawns:

		// Useita uhkaajia.
		// Siispä ei ole mahdollista laittaa eteen mitään.
		if(threats > 1) threatener = NULL;
		return threats;
	}

	void checkOwnMovesToHere(list<Move> &m, short tC, short tR)
	{
		// Samanlainen tarkistus kuin uhkaamisista.
		// Tässä vaan tarkistetaan toisin päin.

		// Up/Down/Left/Right: queens, rooks
		// Up
		for(short r = tR + 1; r < 8; r++)
		{
			if(canMoveOwn(board[r][tC], m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Right
		for(short c = tC + 1; c < 8; c++)
		{
			if(canMoveOwn(board[tR][c], m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Down
		for(short r = tR - 1; r >= 0; r--)
		{
			if(canMoveOwn(board[r][tC], m, ROOK, tC, tR))
			{
				break;
			}
		}
		// Left
		for(short c = tC - 1; c >= 0; c--)
		{
			if(canMoveOwn(board[tR][c], m, ROOK, tC, tR))
			{
				break;
			}
		}

		// Sideways: queens, bishops
		// Up-right
		for(short r = tR + 1, c = tC + 1; r < 8 && c < 8; r++, c++)
		{
			if(canMoveOwn(board[r][c], m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Right-down
		for(short r = tR - 1, c = tC + 1; r >= 0 && c < 8; r--, c++)
		{
			if(canMoveOwn(board[r][c], m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Down-left
		for(short r = tR - 1, c = tC - 1; r >= 0 && c >= 0; r--, c--)
		{
			if(canMoveOwn(board[r][c], m, BISHOP, tC, tR))
			{
				break;
			}
		}
		// Left-up
		for(short r = tR + 1, c = tC - 1; r < 8 && c >= 0; r++, c--)
		{
			if(canMoveOwn(board[r][c], m, BISHOP, tC, tR))
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
			canMoveOwn1Piece(board[r + 2][c + 1], m, KNIGHT, tC, tR);
			// Left
			if(c > 0)
			canMoveOwn1Piece(board[r + 2][c - 1], m, KNIGHT, tC, tR);
		}
		// Right
		if(c < 6)
		{
			// Up
			if(r < 7)
			canMoveOwn1Piece(board[r + 1][c + 2], m, KNIGHT, tC, tR);
			// Down
			if(r > 0)
			canMoveOwn1Piece(board[r - 1][c + 2], m, KNIGHT, tC, tR);
		}
		// Down
		if(r > 1)
		{
			// Right
			if(c < 7)
			canMoveOwn1Piece(board[r - 2][c + 1], m, KNIGHT, tC, tR);
			// Left
			if(c > 0)
			canMoveOwn1Piece(board[r - 2][c - 1], m, KNIGHT, tC, tR);
		}
		// Left
		if(c > 1)
		{
			// Up
			if(r < 7)
			canMoveOwn1Piece(board[r + 1][c - 2], m, KNIGHT, tC, tR);
			// Down
			if(r > 0)
			canMoveOwn1Piece(board[r - 1][c - 2], m, KNIGHT, tC, tR);
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
			{
				canMoveOwn1Piece(board[r][c + 1], m, PAWN, tC, tR);
			}
			// Left side
			if(c > 0)
			{
				canMoveOwn1Piece(board[r][c - 1], m, PAWN, tC, tR);
			}
		}
		else
		{
			if(passer)
			{
				if(c < 7)
					canMoveEnPassant(board[r][c], m, tR, 1);
				if(c > 0)
					canMoveEnPassant(board[r][c], m, tR, -1);
			}
			// Can only move pawn.
			if(canMoveOwnPawn(board[r][c], m, tC, tR))
			{
				if(whoseTurn == WHITE)
				{
					if(r == 2)
					canMoveOwnPawn(board[r - 1][c], m, tC, tR, 3);
				}
				else
				{
					if(r == 5)
					canMoveOwnPawn(board[r + 1][c], m, tC, tR, 3);
				}
			}
		}
	}

	void canKingMove(Piece* p, list<Move> &m, short fC, short fR, short tC, short tR)
	{
		if(p == NULL || p->owner != whoseTurn)
		{
			// Uhataanko tätä ruutua?
			Piece *threatener = NULL;
			if(!isKingThreatened(tC, tR, threatener))
			m.emplace_back(fC, fR, tC, tR);
		}
	}

	bool isClearPath(Piece* p, short col, short row)
	{
		if(p == NULL)
		{
			// Uhataanko tätä ruutua?
			Piece *threatener = NULL;
			return !isKingThreatened(col, row, threatener);
		}
		return false;
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
		Piece *king = &(*(*playersPieces).begin());
		Piece *threatener = NULL;

		// Voiko kuningas liikkua?
		{
			// "Remove" the king from the board to have proper checks.
			board[king->row][king->col] = NULL;

			// First up
			short r = king->row + 1;
			if(r < 8)
			{
				canKingMove(board[r][king->col], moves, king->col, king->row, king->col, r);
			}

			// Then up-right
			short c = king->col + 1;
			if(r < 8 && c < 8)
			{
				canKingMove(board[r][c], moves, king->col, king->row, c, r);
			}

			// Then right
			if(c < 8)
			{
				canKingMove(board[king->row][c], moves, king->col, king->row, c, king->row);
			}

			// Then right-down
			r = king->row - 1;
			if(r >= 0 && c < 8)
			{
				canKingMove(board[r][c], moves, king->col, king->row, c, r);
			}

			// Then down
			if(r >= 0)
			{
				canKingMove(board[r][king->col], moves, king->col, king->row, king->col, r);
			}

			// Then down-left
			c = king->col - 1;
			if(r >= 0 && c >= 0)
			{
				canKingMove(board[r][c], moves, king->col, king->row, c, r);
			}

			// Then left
			if(c >= 0)
			{
				canKingMove(board[king->row][c], moves, king->col, king->row, c, king->row);
			}

			// Then left-up
			r = king->row + 1;
			if(r < 8 && c >= 0)
			{
				canKingMove(board[r][c], moves, king->col, king->row, c, r);
			}

			// Restore the king to the board.
			board[king->row][king->col] = king;
		}

		// Uhataanko?
		if(isKingThreatened(king->col, king->row, threatener))
		{
			cout << "Check!" << endl;

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
							checkOwnMovesToHere(moves, c, r);

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
					checkOwnMovesToHere(moves, col, row);
					break;
				case PAWN:
					// Voiko sotilaan syödä?
					// Kutsu funktiota joka tarkistaa voiko oman napin siirtää tähän kohtaan laudalla.
					checkOwnMovesToHere(moves, col, row);
					break;
				}
			}

			// Muita nappeja ei voi siirtää muualle!
			return moves.size();
		}

		// Tarkista tornitus
		if(whoseTurn == WHITE && (canCastle & 1) || whoseTurn == BLACK && (canCastle & 4))
		{
			for(short c = king->col + 1; c < H; c++)
			{
				if(!isClearPath(board[king->row][c], c, king->row))
				{
					goto EndOfCastlingCheck;
				}
			}
			// Luo tornitussiirto
			moves.emplace_back(E, king->row, G, king->row, 1);
		}
		if(whoseTurn == WHITE && (canCastle & 2) || whoseTurn == BLACK && (canCastle & 8))
		{
			for(short c = king->col - 1; c > B; c--)
			{
				if(!isClearPath(board[king->row][c], c, king->row))
				{
					goto EndOfCastlingCheck;
				}
			}
			// Luo tornitussiirto
			moves.emplace_back(E, king->row, C, king->row, 2);
		}
		EndOfCastlingCheck:

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
						if(moveCheck(board[r][p->col], moves, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then up-right
					for(short r = p->row + 1, c = p->col + 1; r < 8 && c < 8; r++, c++)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then right
					for(short c = p->col + 1; c < 8; c++)
					{
						if(moveCheck(board[p->row][c], moves, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then right-down
					for(short r = p->row - 1, c = p->col + 1; r >= 0 && c < 8; r--, c++)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then down-left
					for(short r = p->row - 1, c = p->col - 1; r >= 0 && c >= 0; r--, c--)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then left
					for(short c = p->col - 1; c >= 0; c--)
					{
						if(moveCheck(board[p->row][c], moves, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
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
						if(moveCheck(board[r][p->col], moves, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then right
					for(short c = p->col + 1; c < 8; c++)
					{
						if(moveCheck(board[p->row][c], moves, p->col, p->row, c, p->row))
						{
							break;
						}
					}

					// Then down
					for(short r = p->row - 1; r >= 0; r--)
					{
						if(moveCheck(board[r][p->col], moves, p->col, p->row, p->col, r))
						{
							break;
						}
					}

					// Then left
					for(short c = p->col - 1; c >= 0; c--)
					{
						if(moveCheck(board[p->row][c], moves, p->col, p->row, c, p->row))
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
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then right-down
					for(short r = p->row - 1, c = p->col + 1; r >= 0 && c < 8; r--, c++)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then left-up
					for(short r = p->row + 1, c = p->col - 1; r < 8 && c >= 0; r++, c--)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
						{
							break;
						}
					}

					// Then down-left
					for(short r = p->row - 1, c = p->col - 1; r >= 0 && c >= 0; r--, c--)
					{
						if(moveCheck(board[r][c], moves, p->col, p->row, c, r))
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
						moveCheck(board[r + 2][c + 1], moves, c, r, c + 1, r + 2);
						// Left
						if(c > 0)
						moveCheck(board[r + 2][c - 1], moves, c, r, c - 1, r + 2);
					}
					// Right
					if(c < 6)
					{
						// Up
						if(r < 7)
						moveCheck(board[r + 1][c + 2], moves, c, r, c + 2, r + 1);
						// Down
						if(r > 0)
						moveCheck(board[r - 1][c + 2], moves, c, r, c + 2, r - 1);
					}
					// Down
					if(r > 1)
					{
						// Right
						if(c < 7)
						moveCheck(board[r - 2][c + 1], moves, c, r, c + 1, r - 2);
						// Left
						if(c > 0)
						moveCheck(board[r - 2][c - 1], moves, c, r, c - 1, r - 2);
					}
					// Left
					if(c > 1)
					{
						// Up
						if(r < 7)
						moveCheck(board[r + 1][c - 2], moves, c, r, c - 2, r + 1);
						// Down
						if(r > 0)
						moveCheck(board[r - 1][c - 2], moves, c, r, c - 2, r - 1);
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
					{
						eatCheckPawn(board[r][c + 1], moves, c, p->row);
						if(passer)
							enPassantCheckPawn(board[p->row][c + 1], moves, c, r);
					}
					// Left side
					if(c > 0)
					{
						eatCheckPawn(board[r][c - 1], moves, c, p->row);
						if(passer)
							enPassantCheckPawn(board[p->row][c - 1], moves, c, r);
					}
					// Move forward
					if(moveCheckPawn(board[r][c], moves, c, p->row, c, r))
					{
						if(whoseTurn == WHITE)
						{
							if(r == 2)
							moveCheckPawn(board[r + 1][c], moves, c, p->row, c, r + 1, 3);
						}
						else
						{
							if(r == 5)
							moveCheckPawn(board[r - 1][c], moves, c, p->row, c, r - 1, 3);
						}
					}
				}
				break;
			}
		}

		return moves.size();
	}

	float evaluate()
	{
		// Normal: 39, max: 103
		short value[6] = {0, 9, 5, 3, 3, 1};
		// Max: 52
		short center[8][8] = {
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 2, 2, 2, 2, 2, 2, 1},
			{1, 2, 3, 3, 3, 3, 2, 1},
			{1, 2, 3, 4, 4, 3, 2, 1},
			{1, 2, 3, 4, 4, 3, 2, 1},
			{1, 2, 3, 3, 3, 3, 2, 1},
			{1, 2, 2, 2, 2, 2, 2, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		};
		// Max: 16
		short promotion[2][8] = {
			{0, 2, 2, 1, 1, 0, 0, 0}, // Black
			{0, 0, 0, 1, 1, 2, 2, 0}  // White
		};

		short matValue = 0, mobValue = 0, pawnValue = 0;
		for(Piece &p: whitePieces)
		{
			matValue += value[p.who];
			mobValue += center[p.col][p.row];
			pawnValue += promotion[p.owner][p.row];
		}
		for(Piece &p: blackPieces)
		{
			matValue -= value[p.who];
			mobValue -= center[p.col][p.row];
			pawnValue -= promotion[p.owner][p.row];
		}
		return 0.005f * (matValue + mobValue + pawnValue);
	}

	void pawnPromotion(Piece* pawn, bool AI)
	{
		Who type;
		if(AI)
		{
			type = QUEEN;
		}
		else
		{
			string cmd;
			cout << "Give the promotion type [Q/N/R/B]" << endl;
			getline(cin, cmd, '\n');
			if(cmd.size() < 1) type = QUEEN;
			switch(cmd[0])
			{
			case 'B':
				type = BISHOP;
				break;
			case 'N':
				type = KNIGHT;
				break;
			case 'Q':
				type = QUEEN;
				break;
			case 'R':
				type = ROOK;
				break;
			default:
				type = QUEEN;
				break;
			}
		}
		list<Piece>* list = (pawn->owner == BLACK) ? &blackPieces : &whitePieces;
		for(Piece &p: *list)
		{
			if(&p == pawn)
			{
				p.who = type;
				return;
			}
		}
	}

	void executeMove(Move &m)
	{
		Piece* to = board[m.toRow][m.toCol];
		if(to != NULL)
		{
			// Delete eatable chess piece.
			list<Piece>* list = (to->owner == BLACK) ? &blackPieces : &whitePieces;
			for(auto i = list->end(); --i != list->begin();)
			{
				if(i->col == m.toCol && i->row == m.toRow)
				{
					list->erase(i);
					break;
				}
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
		board[m.toRow][m.toCol] = from;
		board[m.fromRow][m.fromCol] = NULL;

		if(canCastle)
		switch(from->who)
		{
		case KING:
			if(m.special)
			{
				if(m.special == 1)
				{
					Move kingSide(H, from->row, F, from->row);
					executeMove(kingSide);
				}
				else if(m.special == 2)
				{
					Move queenSide(A, from->row, D, from->row);
					executeMove(queenSide);
				}
			}
			if(from->owner == WHITE)
			{
				canCastle &= ~3;
			}
			else
			{
				canCastle &= ~12;
			}
			passer = NULL;
			return;
		case ROOK:
			if(from->col == H)
			{
				if(from->owner == WHITE)
				canCastle &= ~1;
				else
				canCastle &= ~4;
			}
			else if(from->col == A)
			{
				if(from->owner == WHITE)
				canCastle &= ~2;
				else
				canCastle &= ~8;
			}
			passer = NULL;
			return;
		}

		// Has to be pawn.
		if(m.special >= 3)
		{
			if(m.special == 3)
			{
				cout << "En passant appeared!" << endl;
				passer = from;
				return;
			}
			cout << "Deleting en passant... ";
			list<Piece>* list = (from->owner == WHITE) ? &blackPieces : &whitePieces;
			for(auto i = list->end(); --i != list->begin();)
			{
				if(i->col == passer->col && i->row == passer->row)
				{
					board[passer->row][passer->col] = NULL;
					list->erase(i);
					cout << "done!" << endl;
					break;
				}
			}
		}

		if(from->who == PAWN)
		{
			if(from->owner == WHITE)
			{
				if(from->row == R8)
				{
					pawnPromotion(from, m.AI);
				}
			}
			else
			{
				if(from->row == R1)
				{
					pawnPromotion(from, m.AI);
				}
			}
		}

		// This clears the en passant possibilty.
		passer = NULL;
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
