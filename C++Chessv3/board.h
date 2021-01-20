#pragma once

#include <map>
#include <stdint.h>
#include "movegen.h"

#define SQ_NUM 64
#define BRD_SIZE 8

#define getBit(bb, bit) (bb & (1ULL << bit))
#define setBit(bb, bit) (bb |= (1ULL << bit))
#define popBit(bb, bit) (bb &= ~(1ULL << bit))

#define startPosition "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define trickyPosition "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killerPosition "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

using namespace std;

enum { P, N, B, R, Q, K, p, n, b, r, q, k };
enum { white, black, both };
enum { wk = 1, wq = 2, bk = 4, bq = 8 };

enum Square {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noSq
};

class Position 
{
private:
	uint64_t board[12];
	uint64_t occ[3];
	
	int side;
	int castling;
	int enpassant;
	
public:
	Position();
	
	void print();
	void parseFen(char *fen);
	
	uint64_t getOcc(int side);
	uint64_t getBb(int piece);
	
	int getSide();
	int getCastling();
	int getEnpassant();
	
	bool isSqAttacked(MoveGenerator *movegen, int square, int side);
};

void printBitboard(uint64_t bitboard);
