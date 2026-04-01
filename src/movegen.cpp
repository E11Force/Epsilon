#include "board.hpp"

Bitboard Board::generateKnightMoves(char index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KnightMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KnightMoves = ((IndexMask << 15) & notfileH) | ((IndexMask << 17) & notfileA) | ((IndexMask << 10) & notfileAB) | ((IndexMask >> 6) & notfileAB) | ((IndexMask >> 15) & notfileA) | ((IndexMask >> 17) & notfileH) | ((IndexMask >> 10) & notfileGH) | ((IndexMask << 6) & notfileGH);
	KnightMoves &= ~colorBitboard;
	return KnightMoves;
}

Bitboard Board::generateKingMoves(char index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KingMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KingMoves = ((IndexMask << 8)) | ((IndexMask << 9) & notfileA) | ((IndexMask << 1) & notfileA) | ((IndexMask >> 7) & notfileA) | ((IndexMask >> 8)) | ((IndexMask >> 9) & notfileH) | ((IndexMask >> 1) & notfileH) | ((IndexMask << 7) & notfileH);
	KingMoves &= ~colorBitboard;
	return KingMoves;
}

Bitboard Board::generatePawnMoves(char index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard PawnMoves = 0;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	Bitboard existenceBitboard = byColorBB[White] | byColorBB[Black];
	if (moveTurn == true) {
		PawnMoves |= ((IndexMask << 8) & ~existenceBitboard);
		PawnMoves |= ((PawnMoves << 8) & ~existenceBitboard & rank4);
		PawnMoves |= (((IndexMask << 7) & notfileH & byColorBB[Black]) | ((IndexMask << 9) & notfileA & byColorBB[Black]));
	}
	else if (moveTurn == false) {
		PawnMoves |= ((IndexMask >> 8) & ~existenceBitboard);
		PawnMoves |= ((PawnMoves >> 8) & ~existenceBitboard & rank5);
		PawnMoves |= (((IndexMask >> 7) & notfileA & byColorBB[White]) | ((IndexMask >> 9) & notfileH & byColorBB[White]));
	}
	return PawnMoves;
}

