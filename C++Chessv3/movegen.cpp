#include "movegen.h"
#include "board.h"

const uint64_t NOT_A_FILE = 18374403900871474942ULL;
const uint64_t NOT_H_FILE = 9187201950435737471ULL;
const uint64_t NOT_HG_FILE = 4557430888798830399ULL;
const uint64_t NOT_AB_FILE = 18229723555195321596ULL;

// bishop relevant occupancy bit count for every square on board
const int bishopRelevantBits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const int rookRelevantBits[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

// rook magic numbers
uint64_t rookMagicNumbers[64] = {
	0x8a80104000800020ULL,
	0x140002000100040ULL,
	0x2801880a0017001ULL,
	0x100081001000420ULL,
	0x200020010080420ULL,
	0x3001c0002010008ULL,
	0x8480008002000100ULL,
	0x2080088004402900ULL,
	0x800098204000ULL,
	0x2024401000200040ULL,
	0x100802000801000ULL,
	0x120800800801000ULL,
	0x208808088000400ULL,
	0x2802200800400ULL,
	0x2200800100020080ULL,
	0x801000060821100ULL,
	0x80044006422000ULL,
	0x100808020004000ULL,
	0x12108a0010204200ULL,
	0x140848010000802ULL,
	0x481828014002800ULL,
	0x8094004002004100ULL,
	0x4010040010010802ULL,
	0x20008806104ULL,
	0x100400080208000ULL,
	0x2040002120081000ULL,
	0x21200680100081ULL,
	0x20100080080080ULL,
	0x2000a00200410ULL,
	0x20080800400ULL,
	0x80088400100102ULL,
	0x80004600042881ULL,
	0x4040008040800020ULL,
	0x440003000200801ULL,
	0x4200011004500ULL,
	0x188020010100100ULL,
	0x14800401802800ULL,
	0x2080040080800200ULL,
	0x124080204001001ULL,
	0x200046502000484ULL,
	0x480400080088020ULL,
	0x1000422010034000ULL,
	0x30200100110040ULL,
	0x100021010009ULL,
	0x2002080100110004ULL,
	0x202008004008002ULL,
	0x20020004010100ULL,
	0x2048440040820001ULL,
	0x101002200408200ULL,
	0x40802000401080ULL,
	0x4008142004410100ULL,
	0x2060820c0120200ULL,
	0x1001004080100ULL,
	0x20c020080040080ULL,
	0x2935610830022400ULL,
	0x44440041009200ULL,
	0x280001040802101ULL,
	0x2100190040002085ULL,
	0x80c0084100102001ULL,
	0x4024081001000421ULL,
	0x20030a0244872ULL,
	0x12001008414402ULL,
	0x2006104900a0804ULL,
	0x1004081002402ULL
};

// bishop magic numbers
uint64_t bishopMagicNumbers[64] = {
	0x40040844404084ULL,
	0x2004208a004208ULL,
	0x10190041080202ULL,
	0x108060845042010ULL,
	0x581104180800210ULL,
	0x2112080446200010ULL,
	0x1080820820060210ULL,
	0x3c0808410220200ULL,
	0x4050404440404ULL,
	0x21001420088ULL,
	0x24d0080801082102ULL,
	0x1020a0a020400ULL,
	0x40308200402ULL,
	0x4011002100800ULL,
	0x401484104104005ULL,
	0x801010402020200ULL,
	0x400210c3880100ULL,
	0x404022024108200ULL,
	0x810018200204102ULL,
	0x4002801a02003ULL,
	0x85040820080400ULL,
	0x810102c808880400ULL,
	0xe900410884800ULL,
	0x8002020480840102ULL,
	0x220200865090201ULL,
	0x2010100a02021202ULL,
	0x152048408022401ULL,
	0x20080002081110ULL,
	0x4001001021004000ULL,
	0x800040400a011002ULL,
	0xe4004081011002ULL,
	0x1c004001012080ULL,
	0x8004200962a00220ULL,
	0x8422100208500202ULL,
	0x2000402200300c08ULL,
	0x8646020080080080ULL,
	0x80020a0200100808ULL,
	0x2010004880111000ULL,
	0x623000a080011400ULL,
	0x42008c0340209202ULL,
	0x209188240001000ULL,
	0x400408a884001800ULL,
	0x110400a6080400ULL,
	0x1840060a44020800ULL,
	0x90080104000041ULL,
	0x201011000808101ULL,
	0x1a2208080504f080ULL,
	0x8012020600211212ULL,
	0x500861011240000ULL,
	0x180806108200800ULL,
	0x4000020e01040044ULL,
	0x300000261044000aULL,
	0x802241102020002ULL,
	0x20906061210001ULL,
	0x5a84841004010310ULL,
	0x4010801011c04ULL,
	0xa010109502200ULL,
	0x4a02012000ULL,
	0x500201010098b028ULL,
	0x8040002811040900ULL,
	0x28000010020204ULL,
	0x6000020202d0240ULL,
	0x8918844842082200ULL,
	0x4010011029020020ULL
};

MoveGenerator::MoveGenerator()
{
	initLeaperAttacks();
	initSliderAttacks();
}

int countBits(uint64_t bitboard)
{
	int count = 0;
	
	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}
	
	return count;
}

int getLSBIndex(uint64_t bitboard)
{
	if (bitboard)
		return countBits((bitboard & -bitboard) - 1);
	else
		return -1;
}

void MoveGenerator::initLeaperAttacks()
{
	for (int sq = 0; sq < SQ_NUM; sq++)
	{
		pawnAttacks[white][sq] = maskPawnAttacks(white, sq);
		pawnAttacks[black][sq] = maskPawnAttacks(black, sq);
		kingAttacks[sq] = maskKingAttacks(sq);
		knightAttacks[sq] = maskKnightAttacks(sq);
	}
}

void MoveGenerator::initSliderAttacks()
{
	uint64_t attackMask;
	
	int relevantBitsCount;
	int occupancyIndices;

	for (int sqr = 0; sqr < SQ_NUM; sqr++)
	{
		bishopMasks[sqr] = maskBishopAttacks(sqr);
		rookMasks[sqr] = maskRookAttacks(sqr);

		attackMask = bishopMasks[sqr];
		relevantBitsCount = countBits(attackMask);
		occupancyIndices = (1 << relevantBitsCount);

		for (int index = 0; index < occupancyIndices; index++)
		{
			uint64_t occupancy = setOccupancy(index, relevantBitsCount, attackMask);
			int magicIndex = (occupancy * bishopMagicNumbers[sqr]) >> (64 - bishopRelevantBits[sqr]);
			bishopAttacks[sqr][magicIndex] = bishopAttacksOTF(sqr, occupancy);
		}
		
		attackMask = rookMasks[sqr];
		relevantBitsCount = countBits(attackMask);
		occupancyIndices = (1 << relevantBitsCount);

		for (int index = 0; index < occupancyIndices; index++)
		{
			uint64_t occupancy = setOccupancy(index, relevantBitsCount, attackMask);
			int magicIndex = (occupancy * rookMagicNumbers[sqr]) >> (64 - rookRelevantBits[sqr]);
			rookAttacks[sqr][magicIndex] = rookAttacksOTF(sqr, occupancy);
		}
	}
}

void MoveGenerator::addMove(MoveList *moves, int move)
{
	moves->moves[moves->count++] = move;
}

void MoveGenerator::generateMoves(Position *pos, MoveList *moves)
{
	int fromSquare, toSquare;

	uint64_t bitboard, attacks;

	moves->count = 0;

	for (int piece = P; piece <= k; piece++)
	{
		bitboard = pos->getBb(piece);

		if (pos->getSide() == white)
		{
			if (piece == P)
			{
				while (bitboard)
				{
					fromSquare = getLSBIndex(bitboard);
					toSquare = fromSquare - 8;

					// generate quiet pawn moves
					if (!getBit(pos->getOcc(both), toSquare))
					{
						// promotion
						if (fromSquare >= a7 && fromSquare <= h7)
						{
							addMove(moves, encodeMove(fromSquare, toSquare, piece, Q, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, R, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, B, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, N, 0, 0, 0, 0));
						}
						else
						{
							// single pawn push
							addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));

							// double pawn push
							if ((fromSquare >= a2 && fromSquare <= h2) && !getBit(pos->getOcc(both), toSquare - 8))
								addMove(moves, encodeMove(fromSquare, toSquare - 8, piece, 0, 0, 1, 0, 0));
						}
					}

					attacks = pawnAttacks[white][fromSquare] & pos->getOcc(black);

					// generate pawn captures
					while (attacks)
					{
						toSquare = getLSBIndex(attacks);

						if (fromSquare >= a7 && fromSquare <= h7)
						{
							addMove(moves, encodeMove(fromSquare, toSquare, piece, Q, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, R, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, B, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, N, 1, 0, 0, 0));
						}
						else
							addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

						popBit(attacks, toSquare);
					}

					// generate enpassant captures
					if (pos->getEnpassant() != noSq)
					{
						uint64_t enpassant_attacks = pawnAttacks[white][fromSquare] & (1ULL << pos->getEnpassant());

						if (enpassant_attacks)
						{
							int target_enpassant = getLSBIndex(enpassant_attacks);
							addMove(moves, encodeMove(fromSquare, target_enpassant, piece, 0, 1, 0, 1, 0));
						}
					}

					popBit(bitboard, fromSquare);
				}
			}
			else if (piece == K)
			{
				if (pos->getCastling() & wk)
				{
					if (!getBit(pos->getOcc(both), f1) && !getBit(pos->getOcc(both), g1))
					{
						if (!pos->isSqAttacked(this, e1, black) && !pos->isSqAttacked(this, f1, black))
							addMove(moves, encodeMove(e1, g1, piece, 0, 0, 0, 0, 1));
					}
				}
				if (pos->getCastling() & wq)
				{
					if (!getBit(pos->getOcc(both), d1) && !getBit(pos->getOcc(both), c1) && !getBit(pos->getOcc(both), b1))
					{
						if (!pos->isSqAttacked(this, e1, black) && !pos->isSqAttacked(this, d1, black))
							addMove(moves, encodeMove(e1, c1, piece, 0, 0, 0, 0, 1));
					}
				}
			}
		}
		else
		{
			if (piece == p)
			{
				while (bitboard)
				{
					fromSquare = getLSBIndex(bitboard);
					toSquare = fromSquare + 8;


					// generate quiet pawn moves
					if (!getBit(pos->getOcc(both), toSquare))
					{
						if (fromSquare >= a2 && fromSquare <= h2)
						{
							addMove(moves, encodeMove(fromSquare, toSquare, piece, q, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, r, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, b, 0, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, n, 0, 0, 0, 0));
						}
						else
						{
							// generate single pawn pushes
							addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));

							// generate double pawn pushes
							if ((fromSquare >= a7 && fromSquare <= h7) && !getBit(pos->getOcc(both), toSquare + 8))
								addMove(moves, encodeMove(fromSquare, toSquare + 8, piece, 0, 0, 1, 0, 0));
						}
					}

					attacks = pawnAttacks[black][fromSquare] & pos->getOcc(white);

					// generate pawn captures
					while (attacks)
					{
						toSquare = getLSBIndex(attacks);

						if (fromSquare >= a2 && fromSquare <= h2)
						{
							addMove(moves, encodeMove(fromSquare, toSquare, piece, q, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, r, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, b, 1, 0, 0, 0));
							addMove(moves, encodeMove(fromSquare, toSquare, piece, n, 1, 0, 0, 0));
						}
						else
							addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

						popBit(attacks, toSquare);
					}

					// generate enpassant captures
					if (pos->getEnpassant() != noSq)
					{
						uint64_t enpassant_attacks = pawnAttacks[black][fromSquare] & (1ULL << pos->getEnpassant());

						if (enpassant_attacks)
						{
							int target_enpassant = getLSBIndex(enpassant_attacks);
							addMove(moves, encodeMove(fromSquare, target_enpassant, piece, 0, 1, 0, 1, 0));
						}
					}

					popBit(bitboard, fromSquare);
				}
			}
			else if (piece == k)
			{
				if (pos->getCastling() & bk)
				{
					if (!getBit(pos->getOcc(both), f8) && !getBit(pos->getOcc(both), g8))
					{
						if (!pos->isSqAttacked(this, e8, white) && !pos->isSqAttacked(this, f8, white))
							addMove(moves, encodeMove(e8, g8, piece, 0, 0, 0, 0, 1));
					}
				}
				if (pos->getCastling() & bq)
				{
					if (!getBit(pos->getOcc(both), d8) && !getBit(pos->getOcc(both), c8) && !getBit(pos->getOcc(both), b8))
					{
						if (!pos->isSqAttacked(this, e8, white) && !pos->isSqAttacked(this, d8, white))
							addMove(moves, encodeMove(e8, c8, piece, 0, 0, 0, 0, 1));
					}
				}
			}
		}

		// generate knight moves
		if ((pos->getSide() == white) ? piece == N : piece == n)
		{
			while (bitboard)
			{
				fromSquare = getLSBIndex(bitboard);

				attacks = knightAttacks[fromSquare] & ((pos->getSide() == white) ? ~pos->getOcc(white) : ~pos->getOcc(black));

				while (attacks)
				{
					toSquare = getLSBIndex(attacks);

					// quiet moves
					if (!getBit(((pos->getSide() == white) ? pos->getOcc(black) : pos->getOcc(white)), toSquare))
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));
					// capture moves
					else
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

					popBit(attacks, toSquare);
				}

				popBit(bitboard, fromSquare);
			}
		}

		// generate bishop moves
		if ((pos->getSide() == white) ? piece == B : piece == b)
		{
			while (bitboard)
			{
				fromSquare = getLSBIndex(bitboard);

				attacks = getBishopAttacks(fromSquare, pos->getOcc(both)) & ((pos->getSide() == white) ? ~pos->getOcc(white) : ~pos->getOcc(black));

				while (attacks)
				{
					toSquare = getLSBIndex(attacks);

					// quiet moves
					if (!getBit(((pos->getSide() == white) ? pos->getOcc(black) : pos->getOcc(white)), toSquare))
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));
					// capture moves
					else
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

					popBit(attacks, toSquare);
				}

				popBit(bitboard, fromSquare);
			}
		}

		// generate rook moves
		if ((pos->getSide() == white) ? piece == R : piece == r)
		{
			while (bitboard)
			{
				fromSquare = getLSBIndex(bitboard);

				attacks = getRookAttacks(fromSquare, pos->getOcc(both)) & ((pos->getSide() == white) ? ~pos->getOcc(white) : ~pos->getOcc(black));

				while (attacks)
				{
					toSquare = getLSBIndex(attacks);

					// quiet moves
					if (!getBit(((pos->getSide() == white) ? pos->getOcc(black) : pos->getOcc(white)), toSquare))
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));
					// capture moves
					else
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

					popBit(attacks, toSquare);
				}

				popBit(bitboard, fromSquare);
			}
		}

		// generate queen moves
		if ((pos->getSide() == white) ? piece == Q : piece == q)
		{
			while (bitboard)
			{
				fromSquare = getLSBIndex(bitboard);

				attacks = getQueenAttacks(fromSquare, pos->getOcc(both)) & ((pos->getSide() == white) ? ~pos->getOcc(white) : pos->getOcc(black));

				while (attacks)
				{
					toSquare = getLSBIndex(attacks);

					// quiet moves
					if (!getBit(((pos->getSide() == white) ? pos->getOcc(black) : pos->getOcc(white)), toSquare))
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));
					// capture moves
					else
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

					popBit(attacks, toSquare);
				}

				popBit(bitboard, fromSquare);
			}
		}

		// generate king moves
		if ((pos->getSide() == white) ? piece == K : piece == k)
		{
			while (bitboard)
			{
				fromSquare = getLSBIndex(bitboard);

				attacks = kingAttacks[fromSquare] & ((pos->getSide() == white) ? ~pos->getOcc(white) : ~pos->getOcc(black));

				while (attacks)
				{
					toSquare = getLSBIndex(attacks);

					// quiet moves
					if (!getBit(((pos->getSide() == white) ? pos->getOcc(black) : pos->getOcc(white)), toSquare))
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 0, 0, 0, 0));
					// capture moves
					else
						addMove(moves, encodeMove(fromSquare, toSquare, piece, 0, 1, 0, 0, 0));

					popBit(attacks, toSquare);
				}

				popBit(bitboard, fromSquare);
			}
		}
	}
}

uint64_t MoveGenerator::setOccupancy(int index, int bitsInMask, uint64_t attackMask)
{
	uint64_t occupancy = 0ULL;

	for (int count = 0; count < bitsInMask; count++)
	{
		int square = getLSBIndex(attackMask);

		popBit(attackMask, square);

		if (index & (1 << count))
			occupancy |= (1ULL << square);
	}

	return occupancy;
}

uint64_t MoveGenerator::maskPawnAttacks(int side, int square)
{
	uint64_t attacks = 0ULL;
	uint64_t bitboard = 0ULL;
	
	setBit(bitboard, square);
	
	if (side == white)
	{
		attacks |= (bitboard >> 7) & NOT_A_FILE;
		attacks |= (bitboard >> 9) & NOT_H_FILE;
	}
	else
	{
		attacks |= (bitboard << 7) & NOT_H_FILE;
		attacks |= (bitboard << 9) & NOT_A_FILE;
	}
	
	return attacks;
}

uint64_t MoveGenerator::maskKnightAttacks(int square)
{
	uint64_t bitboard = 0ULL;
	uint64_t attacks = 0ULL;

	setBit(bitboard, square);

	if ((bitboard >> 15) & NOT_A_FILE) attacks |= bitboard >> 15;
	if ((bitboard >> 17) & NOT_H_FILE) attacks |= bitboard >> 17;
	if ((bitboard >> 10) & NOT_HG_FILE) attacks |= bitboard >> 10;
	if ((bitboard >> 6) & NOT_AB_FILE) attacks |= bitboard >> 6;
	if ((bitboard << 15) & NOT_H_FILE) attacks |= bitboard << 15;
	if ((bitboard << 17) & NOT_A_FILE) attacks |= bitboard << 17;
	if ((bitboard << 10) & NOT_AB_FILE) attacks |= bitboard << 10;
	if ((bitboard << 6) & NOT_HG_FILE) attacks |= bitboard << 6;

	return attacks;
}

uint64_t MoveGenerator::maskKingAttacks(int square)
{
	uint64_t bitboard = 0ULL;
	uint64_t attacks = 0ULL;

	setBit(bitboard, square);

	if ((bitboard >> 7) & NOT_A_FILE) attacks |= bitboard >> 7;
	if (bitboard >> 8) attacks |= bitboard >> 8;
	if ((bitboard >> 9) & NOT_H_FILE) attacks |= bitboard >> 9;
	if ((bitboard >> 1) & NOT_H_FILE) attacks |= bitboard >> 1;
	if ((bitboard << 7) & NOT_H_FILE) attacks |= bitboard << 7;
	if (bitboard << 8) attacks |= bitboard << 8;
	if ((bitboard << 9) & NOT_A_FILE) attacks |= bitboard << 9;
	if ((bitboard << 1) & NOT_A_FILE) attacks |= bitboard << 1;

	return attacks;
}

uint64_t MoveGenerator::maskBishopAttacks(int square)
{
	uint64_t attacks = 0ULL;

	int r, f;

	int tr = square / 8;
	int tf = square % 8;

	for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r * 8 + f));
	for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r * 8 + f));
	for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r * 8 + f));

	return attacks;
}

uint64_t MoveGenerator::maskRookAttacks(int square)
{
	uint64_t attacks = 0ULL;

	int r, f;

	int tr = square / 8;
	int tf = square % 8;

	for (r = tr + 1; r <= 6; r++) attacks |= 1ULL << (r * 8 + tf);
	for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
	for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
	for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

	return attacks;
}

uint64_t MoveGenerator::bishopAttacksOTF(int sqr, uint64_t blockers)
{
	uint64_t attacks = 0ULL;

	int r, f;

	int tr = sqr / 8;
	int tf = sqr % 8;

	for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
	{
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & blockers) break;
	}
	for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
	{
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & blockers) break;
	}
	for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
	{
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & blockers) break;
	}
	for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
	{
		attacks |= (1ULL << (r * 8 + f));
		if ((1ULL << (r * 8 + f)) & blockers) break;
	}

	return attacks;
}

uint64_t MoveGenerator::rookAttacksOTF(int square, uint64_t blockers)
{
	uint64_t attacks = 0ULL;

	int r, f;

	int tr = square / 8;
	int tf = square % 8;

	for (r = tr + 1; r <= 7; r++)
	{
		attacks |= (1ULL << (r * 8 + tf));
		if ((1ULL << (r * 8 + tf)) & blockers) break;
	}

	for (r = tr - 1; r >= 0; r--)
	{
		attacks |= (1ULL << (r * 8 + tf));
		if ((1ULL << (r * 8 + tf)) & blockers) break;
	}

	for (f = tf + 1; f <= 7; f++)
	{
		attacks |= (1ULL << (tr * 8 + f));
		if ((1ULL << (tr * 8 + f)) & blockers) break;
	}

	for (f = tf - 1; f >= 0; f--)
	{
		attacks |= (1ULL << (tr * 8 + f));
		if ((1ULL << (tr * 8 + f)) & blockers) break;
	}

	return attacks;
}

uint64_t MoveGenerator::getRookAttacks(int sqr, uint64_t occupancy)
{
	occupancy &= rookMasks[sqr];
	occupancy *= rookMagicNumbers[sqr];
	occupancy >>= 64 - rookRelevantBits[sqr];

	return rookAttacks[sqr][occupancy];
}

uint64_t MoveGenerator::getBishopAttacks(int sqr, uint64_t occupancy)
{
	occupancy &= bishopMasks[sqr];
	occupancy *= bishopMagicNumbers[sqr];
	occupancy >>= 64 - bishopRelevantBits[sqr];

	return bishopAttacks[sqr][occupancy];
}

uint64_t MoveGenerator::getQueenAttacks(int sqr, uint64_t occupancy) 
{ 
	return getBishopAttacks(sqr, occupancy) | getRookAttacks(sqr, occupancy); 
}

