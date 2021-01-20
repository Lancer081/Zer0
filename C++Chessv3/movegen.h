#pragma once

#include <stdint.h>

class MoveGenerator
{
private:
	uint64_t pawnAttacks[2][64];
	uint64_t knightAttacks[64];
	uint64_t kingAttacks[64];
	uint64_t bishopMasks[64];
	uint64_t rookMasks[64];
	uint64_t bishopAttacks[64][512];
	uint64_t rookAttacks[64][4096];
	
public:
	MoveGenerator();
	
	void initLeaperAttacks();
	void initSliderAttacks();
	
	uint64_t maskPawnAttacks(int side, int sq);
	uint64_t maskKnightAttacks(int sq);
	uint64_t maskKingAttacks(int sq);
	uint64_t maskBishopAttacks(int sq);
	uint64_t maskRookAttacks(int sq);
	uint64_t bishopAttacksOTF(int sq, uint64_t block);
	uint64_t rookAttacksOTF(int sq, uint64_t block);
	uint64_t getBishopAttacks(int sq, uint64_t occ);
	uint64_t getRookAttacks(int sq, uint64_t occ);
	uint64_t getQueenAttacks(int sq, uint64_t occ);
	uint64_t setOccupancy(int index, int bitsInMask, uint64_t attackMask);
};

int countBits(uint64_t bitboard);
int getLSBIndex(uint64_t bitboard);
