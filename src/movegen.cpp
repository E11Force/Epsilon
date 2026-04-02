#include "board.hpp"

// Knight
Bitboard Board::generateKnightMoves(Square index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KnightMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KnightMoves = ((IndexMask << 15) & notfileH) | ((IndexMask << 17) & notfileA) | ((IndexMask << 10) & notfileAB) | ((IndexMask >> 6) & notfileAB) | ((IndexMask >> 15) & notfileA) | ((IndexMask >> 17) & notfileH) | ((IndexMask >> 10) & notfileGH) | ((IndexMask << 6) & notfileGH);
	KnightMoves &= ~colorBitboard;
	return KnightMoves;
}

// King
Bitboard Board::generateKingMoves(Square index) {
	Bitboard IndexMask = (1ULL << index);
	Bitboard KingMoves;
	Bitboard colorBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];
	KingMoves = ((IndexMask << 8)) | ((IndexMask << 9) & notfileA) | ((IndexMask << 1) & notfileA) | ((IndexMask >> 7) & notfileA) | ((IndexMask >> 8)) | ((IndexMask >> 9) & notfileH) | ((IndexMask >> 1) & notfileH) | ((IndexMask << 7) & notfileH);
	KingMoves &= ~colorBitboard;
	return KingMoves;
}

// Pawn
Bitboard Board::generatePawnMoves(Square index) {
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

// Rook
Bitboard Board::RookRaycasting(Square index) {
	Bitboard IndexMask(1ULL << index);
	Bitboard RookMoves = 0;
	Bitboard enemyBitboard = moveTurn ? byColorBB[Black] : byColorBB[White];
	Bitboard currentBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];

	while ((IndexMask & notRank1) != 0) {
		IndexMask = IndexMask >> 8;
		if ((IndexMask & ~currentBitboard) == 0) { break; } // if he got into our piece
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notRank8) != 0) {
		IndexMask = IndexMask << 8;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA) != 0) {
		IndexMask = IndexMask >> 1;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileH) != 0) {
		IndexMask = IndexMask << 1;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		RookMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}

	//RookMoves = RookMoves & ~currentBitboard;

	return RookMoves;
}

// Bishop
Bitboard Board::BishopRaycasting(Square index) {
	Bitboard IndexMask(1ULL << index);
	Bitboard BishopMoves = 0;
	Bitboard enemyBitboard = moveTurn ? byColorBB[Black] : byColorBB[White];
	Bitboard currentBitboard = moveTurn ? byColorBB[White] : byColorBB[Black];

	while ((IndexMask & notRank8 & notfileH) != 0) {
		IndexMask = IndexMask << 9;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notRank1 & notfileH) != 0) {
		IndexMask = IndexMask >> 7;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA & notRank1) != 0) {
		IndexMask = IndexMask >> 9;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}
	IndexMask = (1ULL << index);
	while ((IndexMask & notfileA & notRank8) != 0) {
		IndexMask = IndexMask << 7;
		if ((IndexMask & ~currentBitboard) == 0) { break; }
		BishopMoves |= IndexMask;
		if ((IndexMask & ~enemyBitboard) == 0) { break; }
	}

	//BishopMoves = BishopMoves & ~currentBitboard;

	return BishopMoves;
}

// Queen
Bitboard Board::QueenRaycasting(Square index) {
	return RookRaycasting(index) | BishopRaycasting(index);
}

void Board::makeMove(Square sourceIndex, Square destinationIndex) {
	Bitboard sourceMask = (1ULL << sourceIndex);
	Bitboard destinationMask = (1ULL << destinationIndex);

	if (byIndexBB[destinationIndex] != Void) { // if there was an enemy
		byTypeBB[byIndexBB[destinationIndex]] = byTypeBB[byIndexBB[destinationIndex]] & ~destinationMask;
		byColorBB[!moveTurn] = byColorBB[!moveTurn] & ~destinationMask;
	} // continue
	byTypeBB[byIndexBB[sourceIndex]] = byTypeBB[byIndexBB[sourceIndex]] & ~sourceMask | destinationMask;
	byColorBB[moveTurn] = byColorBB[moveTurn] & ~sourceMask | destinationMask;
	byIndexBB[destinationIndex] = byIndexBB[sourceIndex];
	byIndexBB[sourceIndex] = Void;
}

void Board::unmakeMove(Square sourceIndex, Square destinationIndex, Square capturedPiece) {
	Bitboard sourceMask = (1ULL << sourceIndex);
	Bitboard destinationMask = (1ULL << destinationIndex);
	byIndexBB[sourceIndex] = byIndexBB[destinationIndex];
	byIndexBB[destinationIndex] = capturedPiece;
	byTypeBB[byIndexBB[destinationIndex]] = byTypeBB[byIndexBB[destinationIndex]] | destinationMask;
	byTypeBB[byIndexBB[capturedPiece]] = byTypeBB[byIndexBB[capturedPiece]] & ~destinationMask | sourceMask ;
	byColorBB[moveTurn] = byColorBB[moveTurn] | sourceMask;
}