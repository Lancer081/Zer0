#include <iostream>
#include <string.h>
#include "board.h"
#include "movegen.h"

using namespace std;

map<int, char> pieceToChar = {
    {P, 'P'},
    {N, 'N'},
    {B, 'B'},
    {R, 'R'},
    {Q, 'Q'},
    {K, 'K'},
    {p, 'p'},
    {n, 'n'},
    {b, 'b'},
    {r, 'r'},
    {q, 'q'},
    {k, 'k'},
};

map<char, int> charToPiece = {
    {'P', P},
    {'N', N},
    {'B', B},
    {'R', R},
    {'Q', Q},
    {'K', K},
    {'p', p},
    {'n', n},
    {'b', b},
    {'r', r},
    {'q', q},
    {'k', k},
};

Position::Position()
{
	side = 0;
	enpassant = 0;
	castling = 0;

	memset(board, 0ULL, sizeof(board));
	memset(occ, 0ULL, sizeof(occ));
}

int Position::getSide() { return side; }
int Position::getCastling() { return castling; }
int Position::getEnpassant() { return enpassant; }

uint64_t Position::getOcc(int side) { return occ[side]; }
uint64_t Position::getBb(int piece) { return board[piece]; }

void Position::print()
{
	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		int noPiece = 1;
	
		if (!(sq % BRD_SIZE))
			cout << "\n%d " << 8 - (sq / BRD_SIZE);
			
		for (int piece = P; piece <= k; piece++)
		{
			if (getBit(board[piece], sq))
			{
				cout << "%c " << pieceToChar[piece];
				noPiece = 0;
				break;
			}
		}
		
		if (noPiece)
			cout << ". ";
	}
	
	cout << "\n  a b c d e f g h\n\n";
}

void Position::parseFen(char *fen)
{
	side = 0;
	enpassant = noSq;
	castling = 0;

	memset(board, 0, sizeof(board));
	memset(occ, 0, sizeof(occ));

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 8 + file;

			if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
			{
				int piece = charToPiece[*fen];
				setBit(board[piece], square);
				fen++;
			}

			if (*fen >= '0' && *fen <= '9')
			{
				int offset = *fen - '0';
				int target_piece = -1;

				for (int piece = P; piece <= k; piece++)
				{
					if (getBit(board[piece], square))
					{
						target_piece = piece;
						break;
					}
				}

				if (target_piece == -1)
					file--;

				file += offset;

				fen++;
			}

			if (*fen == '/')
				*fen++;
		}
	}

	// go to side to move
	fen++;

	(*fen == 'w') ? (side = white) : (side = black);

	// go to castling rights
	fen += 2;

	while (*fen != ' ')
	{
		switch (*fen)
		{
		case 'K': castling |= wk; break;
		case 'Q': castling |= wq; break;
		case 'k': castling |= bk; break;
		case 'q': castling |= bq; break;
		case '-': break;
		}

		fen++;
	}

	// go to enpassant square
	fen++;

	if (*fen != '-')
	{
		int file = fen[0] - 'a';
		int rank = 8 - (fen[1] - '0');
			
		enpassant = rank * 8 + file;
	}
	else
		enpassant = noSq;

	for (int piece = P; piece <= K; piece++)
		occ[white] |= board[piece];

	for (int piece = p; piece <= k; piece++)
		occ[black] |= board[piece];

	occ[both] = occ[white] | occ[black];

	//hashKey = generate_hash_key();
}

bool Position::isSqAttacked(MoveGenerator *movegen, int square, int side)
{
	if (side == white && (movegen->getPawnAttacks(black, square) & board[P])) return true;
	if (side == black && (movegen->getPawnAttacks(white, square) & board[p])) return true;
	if (movegen->getKnightAttacks(square) & (side == white ? board[N] : board[n])) return true;
	if (movegen->getKingAttacks(square) & (side == white ? board[K] : board[k])) return true;
	if (movegen->getBishopAttacks(square, occ[both]) & (side == white ? board[B] : board[b])) return true;
	if (movegen->getRookAttacks(square, occ[both]) & (side == white ? board[R] : board[r])) return true;
	if (movegen->getQueenAttacks(square, occ[both]) & (side == white ? board[Q] : board[q])) return true;

	return false;
}

void printBitboard(uint64_t bb)
{
	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		if (!(sq % BRD_SIZE))
			cout << endl << BRD_SIZE - (sq / BRD_SIZE) << " ";
			
		cout << (getBit(bb, sq) ? 1 : 0) << " ";
	}
	
	cout << endl << "  a b c d e f g h" << endl << endl;
}
