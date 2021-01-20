#pragma once

#include <stdint.h>
#include "board.h"

#define encodeMove(source, target, piece, promoted, capture, doublePawn, enpassant, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (doublePawn << 21) | \
    (enpassant << 22) | \
    (castling << 23)    \

#define getMoveSource(move) (move & 0x3f)
#define getMoveTarget(move) ((move & 0xfc0) >> 6)
#define getMovePiece(move) ((move & 0xf000) >> 12)
#define getMovePromoted(move) ((move & 0xf0000) >> 16)
#define getMoveCapture(move) (move & 0x100000)
#define getMoveDouble(move) (move & 0x200000)
#define getMoveEnpassant(move) (move & 0x400000)
#define getMoveCastling(move) (move & 0x800000)

#define copyBoard()                                                      \
    uint64_t boardCopy[12], occupancyCopy[3];                            \
    int sideCopy, enpassantCopy, castleCopy;                             \
    memcpy(boardCopy, board, 96);                                		 \
    memcpy(occupancyCopy, occupancy, 24);                            	 \
    sideCopy = side, enpassantCopy = enpassant, castleCopy = castling;                                           

#define takeBack()                                                       \
    memcpy(board, boardCopy, 96);                                		 \
    memcpy(occupancy, occupancyCopy, 24);                                \
    side = sideCopy, enpassant = enpassantCopy, castling = castleCopy;   

typedef struct {
	int moves[256];
	int count;
} MoveList;

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
	
	uint64_t getPawnAttacks(int side, int sq);
	uint64_t getKnightAttacks(int sq);
	uint64_t getKingAttacks(int sq);
	uint64_t getBishopAttacks(int sq, uint64_t occ);
	uint64_t getRookAttacks(int sq, uint64_t occ);
	uint64_t getQueenAttacks(int sq, uint64_t occ);
	
	uint64_t setOccupancy(int index, int bitsInMask, uint64_t attackMask);
	
	void generateMoves(Position *pos, MoveList *moves);
	void addMove(MoveList *moves, int move);
};

int countBits(uint64_t bitboard);
int getLSBIndex(uint64_t bitboard);
